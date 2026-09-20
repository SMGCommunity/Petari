#include "Game/LiveActor/ShadowVolumeFlatModel.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3DGraphAnimator/J3DModelData.hpp>

ShadowVolumeFlatModel::ShadowVolumeFlatModel(const char* pModelName)
    : ShadowVolumeModel("板状シャドウボリューム描画"), mBaseMtx(), mRootDrawMtxIndex(-1), mDropDrawMtxIndex(-1) {
    initVolumeModel(pModelName);
    initJointDrawMtxResister();
}

ShadowVolumeFlatModel::ShadowVolumeFlatModel()
    : ShadowVolumeModel("板状シャドウボリューム描画"), mBaseMtx(), mRootDrawMtxIndex(-1), mDropDrawMtxIndex(-1) {
}

void ShadowVolumeFlatModel::setBaseMatrixPtr(MtxPtr pMtx) {
    mBaseMtx = pMtx;
}

void ShadowVolumeFlatModel::initModel(const char* pModelName) {
    initVolumeModel(pModelName);
    initJointDrawMtxResister();
}

void ShadowVolumeFlatModel::initJointDrawMtxResister() {
    J3DJoint* root = mModelData->getJointTree().getRootNode();
    u16 rootIndex = root->getJntNo();
    u16 dropIndex = root->getChild()->getJntNo();
    mRootDrawMtxIndex = MR::findPosNrmMtxIndexFromModel(mModelData, rootIndex);
    mDropDrawMtxIndex = MR::findPosNrmMtxIndexFromModel(mModelData, dropIndex);
}

void ShadowVolumeFlatModel::draw() const {
    if (getController()->isDraw()) {
        ShadowVolumeDrawer::draw();
    }
}

void ShadowVolumeFlatModel::loadModelDrawMtx() const {
    ShadowController* controller = getController();
    TPos3f rootMtx;
    TPos3f dropMtx;
    TVec3f displacement;
    controller->getDropDir(&displacement);
    displacement.scale(calcBaseDropLength());
    calcRootJoint(&rootMtx, displacement);
    if (controller->isFollowHostScale()) {
        MR::preScaleMtx(rootMtx, controller->getHost()->mScale);
    }

    PSMTXConcat(MR::getCameraViewMtx(), rootMtx, rootMtx);
    GXLoadPosMtxImm(rootMtx, mRootDrawMtxIndex);
    calcDropJoint(&dropMtx, displacement);
    if (controller->isFollowHostScale()) {
        MR::preScaleMtx(dropMtx, controller->getHost()->mScale);
    }

    PSMTXConcat(MR::getCameraViewMtx(), dropMtx, dropMtx);
    GXLoadPosMtxImm(dropMtx, mDropDrawMtxIndex);
}

void ShadowVolumeFlatModel::calcRootJoint(TPos3f* pMtx, const TVec3f& rDisplacement) const {
    pMtx->set(mBaseMtx);
    TVec3f up(mBaseMtx[0][1], mBaseMtx[1][1], mBaseMtx[2][1]);
    if (up.dot(rDisplacement) >= 0.0f) {
        MR::addTransMtx(*pMtx, rDisplacement);
    }
}

void ShadowVolumeFlatModel::calcDropJoint(TPos3f* pMtx, const TVec3f& rDisplacement) const {
    pMtx->set(mBaseMtx);
    TVec3f up(mBaseMtx[0][1], mBaseMtx[1][1], mBaseMtx[2][1]);
    if (up.dot(rDisplacement) < 0.0f) {
        MR::addTransMtx(*pMtx, rDisplacement);
    }
}

ShadowVolumeFlatModel::~ShadowVolumeFlatModel() {
}

#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/ActorJointCtrl.hpp"
#include "Game/LiveActor/LodCtrl.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

ModelObj::ModelObj(const char* pName, const char* pModelName, MtxPtr pMtx, int drawBufferType, int movementType, int calcAnimType, bool a7)
    : LiveActor(pName), mMtx(pMtx) {
    if (drawBufferType < -1) {
        drawBufferType = MR::DrawBufferType_MapObj;
    }

    if (movementType < -1) {
        movementType = MR::MovementType_MapObjDecoration;
    }

    if (calcAnimType < -1) {
        calcAnimType = MR::CalcAnimType_MapObjDecoration;
    }

    initModelManagerWithAnm(pModelName, nullptr, a7);
    MR::connectToScene(this, movementType, calcAnimType, drawBufferType, -1);
    initSound(8, false);
    initEffectKeeper(8, nullptr, false);

    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }
}

void ModelObj::init(const JMapInfoIter&) {
    makeActorAppeared();
}

void ModelObj::calcAndSetBaseMtx() {
    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }
}

void ModelObjNpc::init(const JMapInfoIter& rIter) {
    mLodCtrl = MR::createLodCtrlNPC(this, rIter);
    mJointCtrl = new ActorJointCtrl(this);

    makeActorAppeared();
}

void ModelObjNpc::control() {
    mLodCtrl->update();
    mJointCtrl->update();
}

void ModelObjNpc::calcAndSetBaseMtx() {
    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
        MR::setBaseTRMtx(this, mMtx);
    } else {
        LiveActor::calcAndSetBaseMtx();
    }

    mJointCtrl->setCallBackFunction();
}

ModelObjNpc::~ModelObjNpc() {
}

ModelObjNpc::ModelObjNpc(const char* pName, const char* a2, MtxPtr pMtx) : LiveActor(pName), mMtx(pMtx), mLodCtrl(), mJointCtrl() {
    if (mMtx != nullptr) {
        mPosition.set< f32 >(mMtx[0][3], mMtx[1][3], mMtx[2][3]);
    }

    initModelManagerWithAnm(a2, nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initShadowFromCSV(this, "Shadow");
    initSound(8, false);
    initEffectKeeper(8, nullptr, false);
}

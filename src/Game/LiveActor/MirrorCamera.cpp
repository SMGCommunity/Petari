#include "Game/LiveActor/MirrorCamera.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

void MirrorCamera::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_MirrorCamera, -1, -1, -1);
}

void MirrorCamera::setMirrorMapInfo(const TVec3f& a1, const TVec3f& a2) {
    _18.set< f32 >(a1);
    _C.set< f32 >(a2);
    _24 = -PSVECDotProduct((const Vec*)&_18, (const Vec*)&_C);
}

void MirrorCamera::updateViewMtx() {
    TPos3f cameraInv(MR::getCameraInvViewMtx());
    TVec3f y;
    TVec3f z;
    TVec3f trans;
    TVec3f x;
    cameraInv.getYDirInline(y);
    cameraInv.getZDirInline(z);
    cameraInv.getTransInline(trans);
    y -= _18 * (_18.dot(y) * 2.0f);
    z -= _18 * (_18.dot(z) * 2.0f);
    PSVECCrossProduct(y, z, x);
    trans -= _18 * ((_18.dot(trans) + _24) * 2.0f);
    mViewMtx.setXYZDirInline(x, y, z);
    mViewMtx.setTransInline(trans);
    mViewMtx.invert(mViewMtx);
}

void MirrorCamera::updateModelTexMtx() {
    TProj3f mtx((Mtx44Ptr)MR::getCameraProjectionMtx());
    mtx[2][0] = 0.0f;
    mtx[2][1] = 0.0f;
    mtx[2][2] = -1.0f;
    mtx[2][3] = 0.0f;
    mtx[3][0] = 0.0f;
    mtx[3][1] = 0.0f;
    mtx[3][2] = 0.0f;
    mtx[3][3] = 1.0f;
    PSMTXConcat(mtx, mViewMtx.mMtx, mModelTexMtx.mMtx);
}

f32 MirrorCamera::getDistance(const TVec3f& a1) const {
    TVec3f stack_14;
    TVec3f stack_8;

    stack_8.set< f32 >(_18);
    stack_14.x = stack_8.dot(_C);
    return stack_8.dot(a1) - stack_14.x;
}

namespace MR {
    MirrorCamera* getMirrorCamera() {
        return MR::getSceneObj< MirrorCamera >(SceneObj_MirrorCamera);
    }

    f32 getDistanceToMirror(const TVec3f& rVec) {
        return getMirrorCamera()->getDistance(rVec);
    }
};  // namespace MR

MirrorCamera::~MirrorCamera() {
}

void MirrorCamera::movement() {
    if (MR::isPlayerInAreaObj("MirrorArea")) {
        updateViewMtx();
        updateModelTexMtx();
    }
}

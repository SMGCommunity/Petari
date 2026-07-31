#include "Game/Camera/CameraContext.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

void CameraContext_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
}

CameraContext::~CameraContext() {
}

CameraContext::CameraContext() : NameObj("カメラコンテキスト") {
    initParams();
}

void CameraContext::initParams() {
    // FIXME: float regswap
    // https://decomp.me/scratch/N4sSe

    mView.setPositionFromLookAt(TVec3f(0.0f, 0.0f, 3000.0f), TVec3f(0.0f, 1.0f, 0.0f), TVec3f(0.0f, 0.0f, 0.0f));

    mViewInv.set(mView);
    mViewInv.invert(mViewInv);

    // presumably a "set params" inline?
    mFovy = 45.0f;
    f32 height = MR::getScreenHeight();
    f32 width = MR::getScreenWidth();
    mNearZ = 100.0f;
    mFarZ = 800000.0f;

    mShakeOffset.zero();

    mAspectRatio = width / height;

    updateProjectionMtx();
}

const TPos3f* CameraContext::getViewMtx() const {
    return &mView;
}

const TPos3f* CameraContext::getInvViewMtx() const {
    return &mViewInv;
}

f32 CameraContext::getAspect() const {
    if (MR::isScreen16Per9()) {
        return 16.0f / 9.0f;
    } else {
        return static_cast< f32 >(MR::getScreenWidth()) / MR::getScreenHeight();
    }
}

void CameraContext::setViewMtx(const TPos3f& rMtx, bool b1, bool b2, const TVec3f& rVec) {
    mView.set(rMtx);
    mViewInv.invert(rMtx);
}

void CameraContext::setNearZ(f32 near) {
    mNearZ = near;
    updateProjectionMtx();
}

void CameraContext::setFarZ(f32 far) {
    mFarZ = far;
    updateProjectionMtx();
}

void CameraContext::setFovy(f32 fovy) {
    mFovy = fovy;
    updateProjectionMtx();
}

void CameraContext::setShakeOffset(f32 x, f32 y) {
    mShakeOffset.set(x, y);
    updateProjectionMtx();
}

void CameraContext::updateProjectionMtx() {
    // NON-MATCHING, needs major fixing.
    // TODO
    mProjection.makePerspective(getFovy(), getAspect(), getNearZ(), getFarZ());

    TProj3f trans;
    trans.makeTrans(mShakeOffset.x, mShakeOffset.y);

    mProjection.concat(trans, mProjection);
}

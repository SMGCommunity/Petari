#include "Game/Camera/CameraContext.hpp"

CameraContext::CameraContext()
    : NameObj("カメラコンテキスト") {
    initParams();
}

CameraContext::~CameraContext() {
}

const TPos3f* CameraContext::getViewMtx() const {
    return &mView;
}

const TPos3f* CameraContext::getInvViewMtx() const {
    return &mViewInv;
}

void CameraContext::setNearZ(f32 near) {
    mNearZ = near;
    updateProjectionMtx();
}

void CameraContext::setFovy(f32 fovy) {
    mFovy = fovy;
    updateProjectionMtx();
}

void CameraContext::setShakeOffset(f32 x, f32 y) {
    mShakeOffsetX = x;
    mShakeOffsetY = y;

    updateProjectionMtx();
}
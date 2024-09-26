#include "Game/Camera/CameraContext.hpp"

CameraContext::CameraContext() : NameObj("カメラコンテキスト") {
    initParams();
}

CameraContext::~CameraContext() {

}

const TPos3f *CameraContext::getViewMtx() const {
    return &mView;
}

const TPos3f *CameraContext::getInvViewMtx() const {
    return &mViewInv;
}

void CameraContext::setNearZ(float near) {
    mNearZ = near;
    updateProjectionMtx();
}

void CameraContext::setFovy(float fovy) {
    mFovy = fovy;
    updateProjectionMtx();
}

void CameraContext::setShakeOffset(float x, float y) {
    mShakeOffsetX = x;
    mShakeOffsetY = y;

    updateProjectionMtx();
}
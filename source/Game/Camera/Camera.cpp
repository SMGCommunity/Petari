#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Util/SceneUtil.hpp"

void CamTranslatorDummy::setParam(const CameraParamChunk *pChunk) {

}

Camera *CamTranslatorDummy::getCamera() const {
    return mCamera;
}

Camera::Camera(const char *pName) : NameObj(pName) {
    mPosParam = new CameraPoseParam();
    mVPan = nullptr;
    mIsLOfsErpOff = false;
    mZoneMatrix.identity();
}

void Camera::reset() {
    
}

bool Camera::isInterpolationOff() const {
    return false;
}

bool Camera::isCollisionOff() const {
    return false;
}

bool Camera::isZeroFrameMoveOff() const {
    return false;
}

bool Camera::isSubjectiveCameraOff() const {
    return false;
}

bool Camera::isCorrectingErpPositionOff() const {
    return false;
}

void Camera::roundLeft() {

}

void Camera::roundRight() {

}

bool Camera::isEnableToRoundLeft() const {
    return false;
}

bool Camera::isEnableToRoundRight() const {
    return false;
}

bool Camera::isEnableToReset() const {
    return false;
}

CamTranslatorBase *Camera::createTranslator() {
    return new CamTranslatorDummy(this);
}

#ifdef NON_MATCHING
// mVPan is placed in r3 instead of r0 when comparing with nullptr
void Camera::setZoneMtx(long zoneID) {
    if (zoneID <= 0) {
        mZoneMatrix.identity();
    }
    else {
        mZoneMatrix.identity();

        register TMtx34f *matrix = MR::getZonePlacementMtx(zoneID);
        register Camera *camera = this;

        __asm {
            psq_l   f0, 0x00(matrix), 0, 0
            psq_l   f1, 0x08(matrix), 0, 0
            psq_l   f2, 0x10(matrix), 0, 0
            psq_l   f3, 0x18(matrix), 0, 0
            psq_l   f4, 0x20(matrix), 0, 0
            psq_l   f5, 0x28(matrix), 0, 0
            psq_st  f0, 0x1C(camera), 0, 0
            psq_st  f1, 0x24(camera), 0, 0
            psq_st  f2, 0x2C(camera), 0, 0
            psq_st  f3, 0x34(camera), 0, 0
            psq_st  f4, 0x3C(camera), 0, 0
            psq_st  f5, 0x44(camera), 0, 0
        };
    }

    if (mVPan != nullptr) {
        pan->_60 = 1;
    }
}
#endif

void Camera::createVPanObj() {
    mVPan = new CameraHeightArrange(this);
}
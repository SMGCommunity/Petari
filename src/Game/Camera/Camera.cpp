#include "Game/Camera/Camera.hpp"
#include "Game/Camera/CameraHeightArrange.hpp"
#include "Game/Camera/CameraMan.hpp"
#include "Game/Camera/CameraParamChunk.hpp"
#include "Game/Camera/CameraPoseParam.hpp"
#include "Game/Util/SceneUtil.hpp"

void CamTranslatorDummy::setParam(const CameraParamChunk* pChunk) {}

Camera* CamTranslatorDummy::getCamera() const {
    return mCamera;
}

Camera::Camera(const char* pName) : NameObj(pName) {
    mPoseParam = new CameraPoseParam();
    mVPan = nullptr;
    mIsLOfsErpOff = false;
    mZoneMatrix.identity();
}

void Camera::reset() {}

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

void Camera::roundLeft() {}

void Camera::roundRight() {}

bool Camera::isEnableToRoundLeft() const {
    return false;
}

bool Camera::isEnableToRoundRight() const {
    return false;
}

bool Camera::isEnableToReset() const {
    return false;
}

CamTranslatorBase* Camera::createTranslator() {
    return new CamTranslatorDummy(this);
}

void Camera::setZoneMtx(s32 zoneID) {
    if (zoneID <= 0) {
        mZoneMatrix.identity();
    } else {
        mZoneMatrix.identity();
        TMtx34f *matrix = MR::getZonePlacementMtx(zoneID);
        mZoneMatrix.setInline(*matrix);
    }

    if (doesVPanExist()) {
        mVPan->_60 = 1;
    }
}

void Camera::createVPanObj() {
    mVPan = new CameraHeightArrange(this);
}

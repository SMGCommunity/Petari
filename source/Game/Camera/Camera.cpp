#include "Game/Camera/Camera.h"

CamTranslatorDummy::CamTranslatorDummy(Camera *pCamera) {
    mCamera = pCamera;
}

Camera *CamTranslatorDummy::getCamera() const {
    return mCamera;
}

Camera::Camera(const char *pName) : NameObj(pName) {
    mPosParam = new CameraPoseParam();
    mVPan = NULL;
    _18 = 0;
    mZoneMatrix.identity();
}

CamTranslatorDummy *Camera::createTranslator() {
    return new CamTranslatorDummy(this);
}

void Camera::createVPanObj() {
    mVPan = new CameraHeightArrange(this);
}
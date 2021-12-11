#include "Game/Camera/Camera.h"

Camera::Camera(const char *pName) : NameObj(pName) {
    mPosParam = new CameraPoseParam();
    mVPan = NULL;
    _18 = 0;
    mZoneMatrix.identity();
}

void Camera::createVPanObj() {
    mVPan = new CameraHeightArrange(this);
}
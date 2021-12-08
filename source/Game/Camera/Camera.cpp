#include "Game/Camera/Camera.h"

Camera::Camera(const char *pName) : NameObj(pName) {
    mPosParam = new CameraPoseParam();
    _14 = 0;
    _18 = 0;
    mZoneMatrix.identity();
}
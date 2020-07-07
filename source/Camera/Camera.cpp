#include "Camera/Camera.h"

Camera::Camera(const char *pName) : NameObj(pName)
{
    mParams = new CameraPoseParam();
    _14 = 0;
    _18 = 0;
    mMtx.identity();
}
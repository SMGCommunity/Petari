#include "Game/Camera/CameraMan.h"
#include "Game/Camera/CameraPoseParam.h"

CameraMan::CameraMan(const char *pName) : NameObj(pName) {
    _C = 0;
    mPoseParam = new CameraPoseParam();
    _14 = 0;
    _15 = 0;
    mMatrix.identity();
}
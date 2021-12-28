#include "Game/Camera/CameraMedianTower.h"

CameraMedianTower::CameraMedianTower(const char *pName) : Camera(pName) {
    mString = NULL;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mAngleA = 0.5235988f;
    mUpX = 1200.0f;
    mUpY = 0.0f;
    mUpZ = 0.5f;
    _78 = 0.0f;
    _7C = 0.0f;
    _80 = 1.0f;
    _84 = 1;
    _88 = 120;
}
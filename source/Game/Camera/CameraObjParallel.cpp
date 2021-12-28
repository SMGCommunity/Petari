#include "Game/Camera/CameraObjParallel.h"

CameraObjParallel::CameraObjParallel(const char *pName) : Camera(pName) {
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 3000.0f;
}
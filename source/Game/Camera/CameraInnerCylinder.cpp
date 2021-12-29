#include "Game/Camera/CameraInnerCylinder.h"
#include "Game/Camera/CamTranslatorInnerCylinder.h"

CameraInnerCylinder::CameraInnerCylinder(const char *pName) : Camera(pName) {
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 0.0f;
    mAxis.z = 0.0f;
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 0.0f;
    _74 = 0.0f;
    _78 = 0.0f;
    _7C = 0.0f;
    _80 = 0;
}

CamTranslatorDummy *CameraInnerCylinder::createTranslator() {
    return new CamTranslatorInnerCylinder(this);
}

#include "Game/Camera/CameraFrontAndBack.hpp"
#include "Game/Camera/CamTranslatorFrontAndBack.hpp"

CameraFrontAndBack::CameraFrontAndBack(const char *pName) : Camera(pName) {
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 1200.0f;
}

CameraFrontAndBack::~CameraFrontAndBack() {

}

CamTranslatorBase *CameraFrontAndBack::createTranslator() {
    return new CamTranslatorFrontAndBack(this);
}

#include "Game/Camera/CameraInwardTower.hpp"
#include "Game/Camera/CamTranslatorInwardTower.hpp"

CameraInwardTower::CameraInwardTower(const char *pName) : Camera(pName) {
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mAngleB = 0.0f;
    mAngleA = 0.0f;
    mDist = 2000.0f;
    _70 = 0.0f;
    _74 = 10.0f;
    _78 = 0.0f;
    _7C = 0.0f;
    _80 = 0;

    createVPanObj();
}

CameraInwardTower::~CameraInwardTower() {

}

CamTranslatorBase *CameraInwardTower::createTranslator() {
    return new CamTranslatorInwardTower(this);
}

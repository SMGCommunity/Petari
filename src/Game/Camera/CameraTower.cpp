#include "Game/Camera/CameraTower.hpp"
#include "Game/Camera/CamTranslatorTower.hpp"

CameraTower::CameraTower(const char *pName) : Camera(pName) {
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
    _74 = 0.0f;
    _78 = 0.0f;
    _7C = 0;
    _80 = 0;
    _84 = 0.0f;
    _88 = 0;
    _89 = 0;

    createVPanObj();
}

CameraTower::~CameraTower() {

}

CamTranslatorBase *CameraTower::createTranslator() {
    return new CamTranslatorTower(this);
}

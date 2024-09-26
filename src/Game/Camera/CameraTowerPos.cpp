#include "Game/Camera/CameraTowerPos.hpp"
#include "Game/Camera/CamTranslatorTowerPos.hpp"

CameraTowerPos::CameraTowerPos(const char *pName) : CameraTowerBase(pName) {
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mUpX = 1000.0f;
    mUpY = 0.5f;
    _94 = 0.0f;
    _98 = 0.0f;
    _9C = 0.0f;
}

CameraTowerPos::~CameraTowerPos() {

}

CamTranslatorBase *CameraTowerPos::createTranslator() {
    return new CamTranslatorTowerPos(this);
}

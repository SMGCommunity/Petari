#include "Game/Camera/CameraDPD.hpp"
#include "Game/Camera/CamTranslatorDPD.hpp"

CameraDPD::CameraDPD(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _90 = true;
    _94 = 0;
    mDist = 0.0f;
    _9C = 40.0f;
    mAngleA = 1.5707964f;
    mAngleB = 1.5707964f;
    mWPointZ = 0.0f;
    mWPointX = 0.05f;
    mWPointY = 0.99f;
    _B4 = false;
    mUpX = 0.0f;
    _60.identity();
}

CameraDPD::~CameraDPD() {

}

CamTranslatorBase *CameraDPD::createTranslator() {
    return new CamTranslatorDPD(this);
}
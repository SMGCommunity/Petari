#include "Game/Camera/CameraBlackHole.hpp"
#include "Game/Camera/CamTranslatorBlackHole.hpp"

CameraBlackHole::CameraBlackHole(const char *pName) : Camera(pName) {
    _4C = 1.0f;
    _50 = 0.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAxis.x = 0.0f;
    mAxis.y = 0.0f;
    mAxis.z = 0.0f;
}

CameraBlackHole::~CameraBlackHole() {

}

CamTranslatorBase *CameraBlackHole::createTranslator() {
    return new CamTranslatorBlackHole(this);
}
#include "Game/Camera/CameraFooFighter.hpp"
#include "Game/Camera/CamTranslatorFooFighter.hpp"

CameraFooFighter::CameraFooFighter(const char *pName) : Camera(pName) {
    mAxisY = 300.0f;
    mAxisX = 1200.0f;
    mDist = 0.03f;
    _58 = 0.03f;
    _5C = 0;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
}

CameraFooFighter::~CameraFooFighter() {

}

CamTranslatorBase *CameraFooFighter::createTranslator() {
    return new CamTranslatorFooFighter(this);
}

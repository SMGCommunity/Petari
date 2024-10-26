#include "Game/Camera/CameraCharmedTripodBoss.hpp"
#include "Game/Camera/CamTranslatorCharmedTripodBoss.hpp"

CameraCharmedTripodBoss::CameraCharmedTripodBoss(const char *pName) : Camera(pName) {
    _4C = -1;
    _50 = 0.0f;
    _54 = 1.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 1000.0f;
    _68 = 0.0f;
    _6C = 0.0f;
}

CameraCharmedTripodBoss::~CameraCharmedTripodBoss() {

}

CamTranslatorBase *CameraCharmedTripodBoss::createTranslator() {
    return new CamTranslatorCharmedTripodBoss(this);
}
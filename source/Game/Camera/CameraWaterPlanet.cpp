#include "Game/Camera/CameraWaterPlanet.h"
#include "Game/Camera/CamTranslatorWaterPlanet.h"

CameraWaterPlanet::CameraWaterPlanet(const char *pName) : Camera(pName) {
    mAxisX = 500.0f;
    mAxisY = 2000.0f;
    mAngleA = 0.7853982f;
}

CamTranslatorBase *CameraWaterPlanet::createTranslator() {
    return new CamTranslatorWaterPlanet(this);
}

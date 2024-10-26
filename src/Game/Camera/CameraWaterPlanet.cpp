#include "Game/Camera/CameraWaterPlanet.hpp"
#include "Game/Camera/CamTranslatorWaterPlanet.hpp"

CameraWaterPlanet::CameraWaterPlanet(const char *pName) : Camera(pName) {
    mAxisX = 500.0f;
    mAxisY = 2000.0f;
    mAngleA = 0.7853982f;
}

CameraWaterPlanet::~CameraWaterPlanet() {

}

CamTranslatorBase *CameraWaterPlanet::createTranslator() {
    return new CamTranslatorWaterPlanet(this);
}

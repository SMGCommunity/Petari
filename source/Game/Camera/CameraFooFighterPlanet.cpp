#include "Game/Camera/CameraFooFighterPlanet.hpp"
#include "Game/Camera/CamTranslatorFooFighterPlanet.hpp"

CameraFooFighterPlanet::CameraFooFighterPlanet(const char *pName) : Camera(pName) {
    mAxisX = 500.0f;
    mAxisY = 2000.0f;
    mAngleA = 0.7853982f;
}

CameraFooFighterPlanet::~CameraFooFighterPlanet() {

}

CamTranslatorBase *CameraFooFighterPlanet::createTranslator() {
    return new CamTranslatorFooFighterPlanet(this);
}

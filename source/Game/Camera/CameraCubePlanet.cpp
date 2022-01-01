#include "Game/Camera/CameraCubePlanet.h"
#include "Game/Camera/CamTranslatorCubePlanet.h"

CameraCubePlanet::CameraCubePlanet(const char *pName) : Camera(pName) {
    mDist = 3000.0f;
    mAngleA = 0.5235988f;
    mAngleB = 0.35f;
    _58 = 0.0f;
    _5C = 1.0f;
    _60 = 0.0f;
}

CameraCubePlanet::~CameraCubePlanet() {

}

CamTranslatorBase *CameraCubePlanet::createTranslator() {
    return new CamTranslatorCubePlanet(this);
}

#include "Game/Camera/CameraMedianPlanet.h"
#include "Game/Camera/CamTranslatorMedianPlanet.h"

CameraMedianPlanet::CameraMedianPlanet(const char *pName) : Camera(pName) {
    mString = nullptr;
    mAxisX = 1200.0f;
    mAxisY = 3000.0f;
    mAngleA = 0.7853982f;
    mAngleB = 0.0f;
    mDist = 2000.0f;
    mAxisZ = 0.2f;
    mWPointX = 0.5f;
}

CameraMedianPlanet::~CameraMedianPlanet() {

}

CamTranslatorBase *CameraMedianPlanet::createTranslator() {
    return new CamTranslatorMedianPlanet(this);
}

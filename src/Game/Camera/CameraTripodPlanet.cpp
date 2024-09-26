#include "Game/Camera/CameraTripodPlanet.hpp"
#include "Game/Camera/CamTranslatorTripodPlanet.hpp"

CameraTripodPlanet::CameraTripodPlanet(const char *pName) : Camera(pName) {
    mAxis.x = 0.0f;
    mAxis.y = 1.0f;
    mAxis.z = 0.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 1000.0f;
}

CameraTripodPlanet::~CameraTripodPlanet() {

}

CamTranslatorBase *CameraTripodPlanet::createTranslator() {
    return new CamTranslatorTripodPlanet(this);
}

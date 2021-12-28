#include "Game/Camera/CameraWonderPlanet.h"

CameraWonderPlanet::CameraWonderPlanet(const char *pName) : Camera(pName) {
    mAxisX = 500.0f;
    mAxisY = 2000.0f;
    mAngleA = 0.7853982f;
    _58 = false;
    _5C = 0;
}
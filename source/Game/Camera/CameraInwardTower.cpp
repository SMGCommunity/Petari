#include "Game/Camera/CameraInwardTower.h"

CameraInwardTower::CameraInwardTower(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 1.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 2000.0f;
    _70 = 0.0f;
    _74 = 10.0f;
    _78 = 0.0f;
    _7C = 0.0f;
    _80 = 0;

    createVPanObj();
}
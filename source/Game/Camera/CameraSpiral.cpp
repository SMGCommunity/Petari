#include "Game/Camera/CameraSpiral.h"

CameraSpiral::CameraSpiral(const char *pName) : Camera(pName) {
    _4C = 60;
    _50 = 0;
    _54 = 0;
    _64 = 1000.0f;
    _68 = 1000.0f;
    _6C = 0.0f;
    _70 = 0.0f;
}
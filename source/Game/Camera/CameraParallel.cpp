#include "Game/Camera/CameraParallel.h"

CameraParallel::CameraParallel(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 3000.0f;
    _58 = 0;
    _59 = 0;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 0.0f;

    createVPanObj();
}
#include "Game/Camera/CameraBehind.h"

CameraBehind::CameraBehind(const char *pName) : Camera(pName) {
    _4C = 0.5235988f;
    _50 = 0.0f;
    _54 = 3000.0f;
    createVPanObj();
}
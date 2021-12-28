#include "Game/Camera/CameraBehind.h"

CameraBehind::CameraBehind(const char *pName) : Camera(pName) {
    mAngleB = 0.5235988f;
    mAngleA = 0.0f;
    mDist = 3000.0f;

    createVPanObj();
}
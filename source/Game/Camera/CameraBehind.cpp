#include "Game/Camera/CameraBehind.h"
#include "Game/Camera/CamTranslatorBehind.h"

CameraBehind::CameraBehind(const char *pName) : Camera(pName) {
    mAngleB = 0.5235988f;
    mAngleA = 0.0f;
    mDist = 3000.0f;

    createVPanObj();
}

CamTranslatorBase *CameraBehind::createTranslator() {
    return new CamTranslatorBehind(this);
}
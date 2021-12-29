#include "Game/Camera/CameraParallel.h"
#include "Game/Camera/CamTranslatorParallel.h"

CameraParallel::CameraParallel(const char *pName) : Camera(pName) {
    mAngleB = 0.0f;
    mAngleA = 0.0f;
    mDist = 3000.0f;
    _58 = false;
    _59 = false;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 0.0f;

    createVPanObj();
}

CamTranslatorDummy *CameraParallel::createTranslator() {
    return new CamTranslatorParallel(this);
}

#include "Game/Camera/CameraObjParallel.h"
#include "Game/Camera/CamTranslatorObjParallel.h"

CameraObjParallel::CameraObjParallel(const char *pName) : Camera(pName) {
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 3000.0f;
}

CamTranslatorDummy *CameraObjParallel::createTranslator() {
    return new CamTranslatorObjParallel(this);
}

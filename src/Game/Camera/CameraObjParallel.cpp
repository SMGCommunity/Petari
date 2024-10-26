#include "Game/Camera/CameraObjParallel.hpp"
#include "Game/Camera/CamTranslatorObjParallel.hpp"

CameraObjParallel::CameraObjParallel(const char *pName) : Camera(pName) {
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 3000.0f;
}

CameraObjParallel::~CameraObjParallel() {

}

CamTranslatorBase *CameraObjParallel::createTranslator() {
    return new CamTranslatorObjParallel(this);
}

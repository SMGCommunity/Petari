#include "Game/Camera/CameraParallel.hpp"
#include "Game/Camera/CamTranslatorParallel.hpp"

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

CameraParallel::~CameraParallel() {

}

CamTranslatorBase *CameraParallel::createTranslator() {
    return new CamTranslatorParallel(this);
}

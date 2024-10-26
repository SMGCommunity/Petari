#include "Game/Camera/CameraGround.hpp"
#include "Game/Camera/CamTranslatorGround.hpp"

CameraGround::CameraGround(const char *pName) : Camera(pName) {
    mAngleA = 0.0f;
    mAngleB = 0.0f;
    mDist = 1000.0f;
    mUpX = 0.0f;
    mUpY = 1.0f;
    mUpZ = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0.0f;
    _70 = true;
}

CameraGround::~CameraGround() {

}

CamTranslatorBase *CameraGround::createTranslator() {
    return new CamTranslatorGround(this);
}
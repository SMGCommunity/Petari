#include "Game/Camera/CameraInwardSphere.hpp"
#include "Game/Camera/CamTranslatorInwardSphere.hpp"

CameraInwardSphere::CameraInwardSphere(const char *pName) : Camera(pName) {
    mDist = 1500.0f;
    mAngleA = 500.0f;
    mAngleB = 300.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0.0f;
}

CameraInwardSphere::~CameraInwardSphere() {

}

CamTranslatorBase *CameraInwardSphere::createTranslator() {
    return new CamTranslatorInwardSphere(this);
}

#include "Game/Camera/CameraSlide.hpp"
#include "Game/Camera/CamTranslatorSlide.hpp"

CameraSlide::CameraSlide(const char *pName) : Camera(pName) {
    mAxis.x = 1.0f;
    mAxis.y = 0.0f;
    mAxis.z = 0.0f;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 0.0f;
    mAngleA = 0.0f;
    mDist = 1000.0f;
}

CameraSlide::~CameraSlide() {

}

CamTranslatorBase *CameraSlide::createTranslator() {
    return new CamTranslatorSlide(this);
}

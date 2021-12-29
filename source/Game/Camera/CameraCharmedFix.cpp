#include "Game/Camera/CameraCharmedFix.h"
#include "Game/Camera/CamTranslatorCharmedFix.h"

CameraCharmedFix::CameraCharmedFix(const char *pName) : Camera(pName) {
    mAxis.x = 0.0f;
    mAxis.y = 0.0f;
    mAxis.z = 0.0f;
    mUp.x = 0.0f;
    mUp.y = 1.0f;
    mUp.z = 0.0f;
    mWPoint.x = 0.0f;
    mWPoint.y = 0.0f;
    mWPoint.z = 1000.0f;
}

CamTranslatorBase *CameraCharmedFix::createTranslator() {
    return new CamTranslatorCharmedFix(this);
}

#include "Game/Camera/CameraCharmedVecReg.h"
#include "Game/Camera/CamTranslatorCharmedVecReg.h"

CameraCharmedVecReg::CameraCharmedVecReg(const char *pName) : Camera(pName) {
    mString = nullptr;
    mDist = 1000.0f;
    mAxisX = 0.0f;
    mAxisZ = 0.0f;
    mAxisY = 0.0f;
    mAngleA = 0.5f;
    mAngleB = 0.02f;
}

CameraCharmedVecReg::~CameraCharmedVecReg() {

}

CamTranslatorBase *CameraCharmedVecReg::createTranslator() {
    return new CamTranslatorCharmedVecReg(this);
}
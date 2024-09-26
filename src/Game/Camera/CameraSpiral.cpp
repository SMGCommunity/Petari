#include "Game/Camera/CameraSpiral.hpp"
#include "Game/Camera/CamTranslatorSpiral.hpp"

CameraSpiral::CameraSpiral(const char *pName) : Camera(pName) {
    _4C = 60;
    _50 = 0;
    _54 = 0;
    mWPointZ = 1000.0f;
    mAxisZ = 1000.0f;
    mWPointX = 0.0f;
    mAxisX = 0.0f;
}

CameraSpiral::~CameraSpiral() {

}

CamTranslatorBase *CameraSpiral::createTranslator() {
    return new CamTranslatorSpiral(this);
}

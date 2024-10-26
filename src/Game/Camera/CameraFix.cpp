#include "Game/Camera/CameraFix.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/Camera/CamTranslatorFix.hpp"

CameraFix::CameraFix(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 0.0f;
    _60 = -2500.0f;
    _64 = 0.0f;
    _68 = 1.0f;
    _6C = 0.0f;
    mTarget = new CameraTargetMtx("カメラターゲットダミー");
}

CameraFix::~CameraFix() {

}

CamTranslatorBase *CameraFix::createTranslator() {
    return new CamTranslatorFix(this);
}

#include "Game/Camera/CameraRailFollow.h"
#include "Game/Camera/CamTranslatorRailFollow.h"

CameraRailFollow::CameraRailFollow(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 30.0f;
    _60 = 0.35f;
}

CamTranslatorBase *CameraRailFollow::createTranslator() {
    return new CamTranslatorRailFollow(this);
}

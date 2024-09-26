#include "Game/Camera/CameraRailFollow.hpp"
#include "Game/Camera/CamTranslatorRailFollow.hpp"

CameraRailFollow::CameraRailFollow(const char *pName) : Camera(pName) {
    _4C = 0;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 30.0f;
    _60 = 0.35f;
}

CameraRailFollow::~CameraRailFollow() {

}

CamTranslatorBase *CameraRailFollow::createTranslator() {
    return new CamTranslatorRailFollow(this);
}

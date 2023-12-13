#include "Game/Camera/CameraFollow.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Camera/CamTranslatorFollow.hpp"

bool CameraTargetObj::isJumping() const {
    return false;
}

CameraFollow::CameraFollow(const char *pName) : Camera(pName) {
    _4C = 1200.0f;
    _50 = 300.0f;
    _54 = 0.17453294f;
    _58 = 0.34906587f;
    _5C = 0.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0.0f;
    _70 = 0.0f;
    _74 = 0.15f;
    _78 = 0.0f;
    _7C = 1;
    _7D = 0;
    _80 = 0.0f;
    _84 = 0.0f;
    _88 = 0;
    _89 = 0;
    createVPanObj();
}

CameraFollow::~CameraFollow() {

}

CamTranslatorBase *CameraFollow::createTranslator() {
    return new CamTranslatorFollow(this);
}

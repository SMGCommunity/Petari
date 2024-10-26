#include "Game/Camera/CameraDead.hpp"
#include "Game/Camera/CameraTargetObj.hpp"
#include "Game/Camera/CamTranslatorDead.hpp"

f32 CameraTargetObj::getRadius() const {
    return 150.0f;
}

CameraDead::CameraDead(const char *pName) : Camera(pName) {
    mDist = 0.5f;
    _54 = 0;
    _58 = 0;
    _5C = 0;
}

CameraDead::~CameraDead() {

}

CamTranslatorBase *CameraDead::createTranslator() {
    return new CamTranslatorDead(this);
}

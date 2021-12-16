#include "Game/Camera/CameraDead.h"
#include "Game/Camera/CameraTargetObj.h"

f32 CameraTargetObj::getRadius() const {
    return 150.0f;
}

CameraDead::CameraDead(const char *pName) : Camera(pName) {
    _50 = 0.5f;
    _54 = 0;
    _58 = 0;
    _5C = 0;
}
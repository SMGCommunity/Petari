#include "Game/Camera/CameraSubjective.h"

CameraSubjective::CameraSubjective(const char *pName) : Camera(pName) {
    _4C = 0.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C.identity();
}
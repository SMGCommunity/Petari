#include "Game/Camera/CameraSlide.h"

CameraSlide::CameraSlide(const char *pName) : Camera(pName) {
    _4C = 1.0f;
    _50 = 0.0f;
    _54 = 0.0f;
    _58 = 0.0f;
    _5C = 1.0f;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0.0f;
    _70 = 0.0f;
    _74 = 1000.0f;
}
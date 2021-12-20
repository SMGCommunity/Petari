#include "Game/Camera/CameraWaterFollow.h"

CameraWaterFollow::CameraWaterFollow(const char *pName) : Camera(pName) {
    _4C = 300.0f;
    _50 = 1200.0f;
    _54 = 0.01f;
    _58 = 0.01f;
    _5C = 0;
    _60 = 0.0f;
    _64 = 0.0f;
    _68 = 0.0f;
    _6C = 0;
    _70 = 0;
    _74 = 1.0f;
    _78 = 0.0f;
    _7C = 0.0f;
}
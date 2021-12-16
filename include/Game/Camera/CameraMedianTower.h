#pragma once

#include "Game/Camera/Camera.h"

class CameraMedianTower : public Camera {
public:
    CameraMedianTower(const char *);

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    f32 _80;
    u8 _84;
    u8 _85[3];
    u32 _88;
};
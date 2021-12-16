#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterFollow : public Camera {
public:
    CameraWaterFollow(const char *);

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    u8 _6C;
    u8 _6D[3];
    u32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
};
#pragma once

#include "Game/Camera/Camera.h"

class CameraRailWatch : public Camera {
public:
    CameraRailWatch(const char *);

    u32 _4C;
    u32 _50;
    u32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
};
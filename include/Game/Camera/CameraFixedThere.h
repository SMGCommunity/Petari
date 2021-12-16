#pragma once

#include "Game/Camera/Camera.h"

class CameraFixedThere : public Camera {
public:
    CameraFixedThere(const char *);

    virtual void calc();

    u32 _4C;
    u8 _50;
    u8 _51[3];
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
};
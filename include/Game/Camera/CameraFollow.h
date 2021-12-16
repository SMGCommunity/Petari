#pragma once

#include "Game/Camera/Camera.h"

class CameraFollow : public Camera {
public:
    CameraFollow(const char *);

    virtual void calc();

    f32 _4C;
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
    u8 _7C;
    u8 _7D;
    u8 _7E[2];
    f32 _80;
    f32 _84;
    u8 _88;
    u8 _89;
    u8 _8A[2];
};
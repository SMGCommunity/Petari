#pragma once

#include "Game/Camera/Camera.h"

class CameraSpiral : public Camera {
public:
    CameraSpiral(const char *);

    virtual CameraTargetObj *calc();

    u32 _4C;
    u32 _50;
    u32 _54;
    u8 _58[12];
    f32 _64;
    f32 _68;
    f32 _6C;
    f32 _70;
};
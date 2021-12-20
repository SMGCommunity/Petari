#pragma once

#include "Game/Camera/Camera.h"

class CameraRailFollow : public Camera {
public:
    CameraRailFollow(const char *);

    virtual CameraTargetObj *calc();

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    u8 _64[4];
};
#pragma once

#include "Game/Camera/Camera.h"

class CameraTrundle : public Camera {
public:
    CameraTrundle(const char *);

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
};
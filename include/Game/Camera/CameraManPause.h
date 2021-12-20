#pragma once

#include "Game/Camera/CameraMan.h"

class CameraManPause : public CameraMan {
public:
    CameraManPause(const char *);

    f32 _48;
    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
};
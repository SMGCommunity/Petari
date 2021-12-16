#pragma once

#include "Game/Camera/Camera.h"

class CameraMedianPlanet : public Camera {
public:
    CameraMedianPlanet(const char *);

    u32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
};
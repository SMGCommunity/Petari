#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterPlanet : public Camera {
public:
    CameraWaterPlanet(const char *);

    f32 _4C;
    f32 _50;
    f32 _54;
};
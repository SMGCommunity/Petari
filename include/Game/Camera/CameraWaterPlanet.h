#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterPlanet : public Camera {
public:
    CameraWaterPlanet(const char *);

    virtual void calc();

    f32 _4C;
    f32 _50;
    f32 _54;
};
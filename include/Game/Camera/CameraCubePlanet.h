#pragma once

#include "Game/Camera/Camera.h"

class CameraCubePlanet : public Camera {
public:
    CameraCubePlanet(const char *);

    virtual void calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
};
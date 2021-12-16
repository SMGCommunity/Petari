#pragma once

#include "Game/Camera/Camera.h"

class CameraSubjective : public Camera {
public:
    CameraSubjective(const char *);

    virtual void calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    TMtx34f _5C;
};
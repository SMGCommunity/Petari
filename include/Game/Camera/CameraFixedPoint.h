#pragma once

#include "Game/Camera/Camera.h"

class CameraFixedPoint : public Camera {
public:
    CameraFixedPoint(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58[4];
};
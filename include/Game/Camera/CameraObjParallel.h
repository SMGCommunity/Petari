#pragma once

#include "Game/Camera/Camera.h"

class CameraObjParallel : public Camera {
public:
    CameraObjParallel(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
};
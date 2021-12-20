#pragma once

#include "Game/Camera/Camera.h"

class CameraRailDemo : public Camera {
public:
    CameraRailDemo(const char *);

    virtual CameraTargetObj *calc();

    u8 _4C[24];
};
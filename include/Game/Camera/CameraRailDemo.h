#pragma once

#include "Game/Camera/Camera.h"

class CameraRailDemo : public Camera {
public:
    CameraRailDemo(const char *);

    virtual void calc();

    u8 _4C[24];
};
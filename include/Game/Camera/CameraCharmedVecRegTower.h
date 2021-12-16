#pragma once

#include "Game/Camera/Camera.h"

class CameraCharmedVecRegTower : public Camera {
public:
    CameraCharmedVecRegTower(const char *);

    u8 _4C[36];
    u8 _70;
    u8 _71[7];
};
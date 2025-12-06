#pragma once

#include "revolution.h"

class CameraMan;

class CamHeliEffector {
public:
    CamHeliEffector();

    void update(CameraMan*);

    u32 _0;
    f32 _4;
};
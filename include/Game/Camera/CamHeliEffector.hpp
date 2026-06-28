#pragma once

#include "revolution/types.h"

class CameraMan;

class CamHeliEffector {
public:
    CamHeliEffector();

    void update(CameraMan*);

    s32 _0;
    f32 _4;
};

#pragma once

#include "Game/Camera/Camera.h"

class CameraWaterPlanetBoss : public Camera {
public:
    CameraWaterPlanetBoss(const char *);

    virtual void calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    u32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    u8 _6C;
    u8 _6D[3];
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    f32 _80;
    f32 _84;
    f32 _88;
    u8 _8C;
    u8 _8D[3];
    u32 _90;
    f32 _94;
    f32 _98;
    f32 _9C;
};
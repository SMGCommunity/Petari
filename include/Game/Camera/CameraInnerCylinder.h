#pragma once

#include "Game/Camera/Camera.h"

class CameraInnerCylinder : public Camera {
public:
    CameraInnerCylinder(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    f32 _58;
    f32 _5C;
    f32 _60;
    f32 _64;
    f32 _68;
    f32 _6C;
    u8 _70[4];    
    f32 _74;
    f32 _78;
    f32 _7C;
    u8 _80;
    u8 _81[3];
};
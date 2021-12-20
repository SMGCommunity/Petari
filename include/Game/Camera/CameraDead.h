#pragma once

#include "Game/Camera/Camera.h"
#include "Game/NameObj/NameObj.h"

class OnlyCamera;

class CameraDead : public Camera {
public:
    CameraDead(const char *);

    virtual CameraTargetObj *calc();

    u8 _4C[4];
    f32 _50;
    u32 _54;
    u32 _58;
    u32 _5C;    
};

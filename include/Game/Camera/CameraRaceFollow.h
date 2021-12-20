#pragma once

#include "Game/Camera/Camera.h"

class CameraRaceFollow : public Camera {
public:
    CameraRaceFollow(const char *);

    virtual CameraTargetObj *calc();

    f32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59[3];
    f32 _5C;
};
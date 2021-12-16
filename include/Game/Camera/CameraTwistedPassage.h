#pragma once

#include "Game/Camera/Camera.h"

class CameraTwistedPassage : public Camera {
public:
    CameraTwistedPassage(const char *);

    virtual void calc();

    u32 _4C;
    f32 _50;
    f32 _54;
    u8 _58;
    u8 _59[3];
};
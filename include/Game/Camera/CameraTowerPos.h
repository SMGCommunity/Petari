#pragma once

#include "Game/Camera/CameraTowerBase.h"

class CameraTowerPos : public CameraTowerBase {
public:
    CameraTowerPos(const char *);

    virtual void calc();

    f32 _6C;
    f32 _70;
    f32 _74;
    f32 _78;
    f32 _7C;
    f32 _80;
    f32 _84;
    f32 _88;
    f32 _8C;
    f32 _90;
    f32 _94;
    f32 _98;
    f32 _9C;
};
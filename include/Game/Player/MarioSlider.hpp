#pragma once

#include "Game/Player/MarioState.hpp"
#include "revolution/mtx.h"

class MarioActor;

class MarioSlider : public MarioState {
public:
    MarioSlider(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool postureCtrl(MtxPtr);

    void calcGroundAccel();
    void calcWallHit();

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    f32 _38;
    f32 _3C;
    u16 _40;
};

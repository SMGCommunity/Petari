#pragma once

#include "Game/Player/MarioState.hpp"
#include "revolution/mtx.h"

class MarioActor;

class MarioSlider : public MarioState {
public:
    MarioSlider(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();
    /* 0x28 */ virtual bool postureCtrl(MtxPtr);

    void calcGroundAccel();
    void calcWallHit();

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    f32 _38;
    f32 _3C;
    u16 _40;
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStick : public MarioState {
public:
    MarioStick(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();
    /* 0x20 */ virtual bool notice();
    /* 0x28 */ virtual bool postureCtrl(MtxPtr);

    bool startJump();
    bool setStickSensor(const HitSensor*);

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    TVec3f _38;
    f32 _44;
    Triangle* _48;
    u32 _4C;
    u8 _50;
    u8 _51;
    u8 _52;
    u8 _53;
    TVec3f _54;
    TVec3f _60;
    TVec3f _6C;
    TVec3f _78;
};

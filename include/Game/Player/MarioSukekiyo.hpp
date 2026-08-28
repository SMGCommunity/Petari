#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSukekiyo : public MarioState {
public:
    MarioSukekiyo(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();
    /* 0x20 */ virtual bool notice();
    /* 0x28 */ virtual bool postureCtrl(MtxPtr);

    TVec3f _14;
    TVec3f _20;
    TVec3f _2C;
    TVec3f _38;
    Triangle* _44;
    u16 _48;
    u8 _4A;
};

class MarioBury : public MarioSukekiyo {
public:
    MarioBury(MarioActor*);
};

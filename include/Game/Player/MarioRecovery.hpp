#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioRecovery : public MarioState {
public:
    MarioRecovery(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    bool calcFirstVector();
    void updateJump();

    /* 0x11 */ u8 _11;
    /* 0x12 */ u8 _12;
    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 _16;
    /* 0x18 */ u16 _18;
    /* 0x1A */ u16 _1A;
    /* 0x1C */ TVec3f _1C;
    /* 0x28 */ TVec3f _28;
    /* 0x34 */ TVec3f _34;
    /* 0x40 */ TVec3f _40;
    /* 0x4C */ TVec3f _4C;
    /* 0x58 */ TVec3f _58;
    /* 0x64 */ TVec3f _64;
    /* 0x70 */ TVec3f _70;
    /* 0x7C */ f32 _7C;
    /* 0x80 */ f32 _80;
    /* 0x84 */ f32 _84;
    /* 0x88 */ u16 _88;
};

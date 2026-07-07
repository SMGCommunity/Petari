#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWarp : public MarioState {
public:
    MarioWarp(MarioActor*);

    virtual bool start();
    virtual bool update();
    virtual bool close();

    void calcAxis();
    void updateJump();

    /* 0x12 */ u8 _12;
    /* 0x14 */ TVec3f _14;
    /* 0x20 */ TVec3f _20;
    /* 0x2C */ TVec3f _2C;
    /* 0x38 */ TVec3f _38;
    /* 0x44 */ u8 _44;
    /* 0x45 */ u8 _45;
    /* 0x48 */ u32 _48;
    /* 0x4C */ LiveActor* _4C;
    /* 0x50 */ bool _50;
    /* 0x52 */ u16 _52;
    /* 0x54 */ u16 _54;
    /* 0x56 */ u16 _56;
    /* 0x58 */ u16 _58;
    /* 0x5C */ TVec3f _5C;
    /* 0x68 */ TVec3f _68;
    /* 0x74 */ TVec3f _74;
    /* 0x80 */ f32 _80;
    /* 0x84 */ f32 _84;
    /* 0x88 */ s32 _88;
    /* 0x8C */ s32 _8C;
};

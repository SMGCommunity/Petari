#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioRabbit : public MarioState {
public:
    MarioRabbit(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice() {
        return true;
    }

    void hop();
    void forceJump();
    void impact();

    /* 0x14 */ f32 _14;
    /* 0x18 */ TVec3f _18;
    /* 0x24 */ u8 _24;
    /* 0x25 */ u8 _25;
    /* 0x26 */ u8 _26;
    /* 0x27 */ u8 _27;
    /* 0x28 */ u8 _28;
    /* 0x2A */ u16 _2A;
    /* 0x2C */ Mtx _2C;
    /* 0x5C */ TVec3f _5C;
    /* 0x68 */ u8 _68;
    /* 0x69 */ u8 _69;
    /* 0x6A */ u8 _6A;
};

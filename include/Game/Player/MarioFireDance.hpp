#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFireDance : public MarioState {
public:
    MarioFireDance(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    void impact();
    void impactEffect();

    /* 0x14 */ TVec3f _14;
    /* 0x20 */ f32 _20;
    /* 0x24 */ u8 _24;
    /* 0x26 */ u16 _26;
    /* 0x28 */ u8 _28;
    /* 0x29 */ u8 _29;
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBump : public MarioState {
public:
    MarioBump(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    /* 0x12 */ u16 _12;
    /* 0x14 */ f32 _14;
    /* 0x18 */ TVec3f _18;
};

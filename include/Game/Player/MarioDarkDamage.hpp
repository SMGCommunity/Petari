#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioDarkDamage : public MarioState {
public:
    MarioDarkDamage(MarioActor*);

    virtual bool start();
    virtual bool update();
    virtual bool notice();

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
};

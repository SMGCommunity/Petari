#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioClimb : public MarioState {
public:
    MarioClimb(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    /* 0x12 */ u16 mTimer;
};

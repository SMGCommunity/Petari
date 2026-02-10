#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioRabbit : public MarioState {
public:
    MarioRabbit(MarioActor*);
    void hop();
    void forceJump();

    u8 _11[0x5B];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFireDamage : public MarioState {
public:
    MarioFireDamage(MarioActor*);

    void decAfterTimer();

    u16 _12;
};

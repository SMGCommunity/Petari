#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioClimb : public MarioState {
public:
    MarioClimb(MarioActor*);

    u8 _11[3];
};

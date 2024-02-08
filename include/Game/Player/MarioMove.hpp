#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioMove : public MarioState {
public:
    MarioMove(MarioActor *);

    u8 _11[0x47];
};

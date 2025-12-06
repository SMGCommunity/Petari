#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFireDance : public MarioState {
public:
    MarioFireDance(MarioActor*);

    u8 _11[0x1B];
};

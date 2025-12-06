#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFoo : public MarioState {
public:
    MarioFoo(MarioActor*);

    u8 _11[0x6AB];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBury : public MarioState {
public:
    MarioBury(MarioActor *);

    u8 _11[0x3B];
};

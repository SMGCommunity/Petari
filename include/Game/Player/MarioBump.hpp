#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBump : public MarioState {
public:
    MarioBump(MarioActor*);

    u8 _11[0x13];
};

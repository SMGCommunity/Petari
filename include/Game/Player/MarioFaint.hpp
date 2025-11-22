#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFaint : public MarioState {
public:
    MarioFaint(MarioActor*);

    u8 _11[0x17];
};

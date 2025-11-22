#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioTeresa : public MarioState {
public:
    MarioTeresa(MarioActor*);

    u8 _11[0x4B];
};

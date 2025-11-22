#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStick : public MarioState {
public:
    MarioStick(MarioActor*);

    u8 _11[0x73];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSideStep : public MarioState {
public:
    MarioSideStep(MarioActor*);

    u8 _11[3];
};

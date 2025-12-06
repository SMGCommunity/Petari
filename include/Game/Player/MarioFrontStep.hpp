#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFrontStep : public MarioState {
public:
    MarioFrontStep(MarioActor*);

    u8 _11[3];
};

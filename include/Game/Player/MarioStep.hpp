#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStep : public MarioState {
public:
    MarioStep(MarioActor*);
    u8 _11[11];
};

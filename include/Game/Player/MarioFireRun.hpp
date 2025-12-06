#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFireRun : public MarioState {
public:
    MarioFireRun(MarioActor*);
    u8 _11[11];
};

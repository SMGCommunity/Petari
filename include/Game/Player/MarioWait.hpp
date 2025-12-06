#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWait : public MarioState {
public:
    MarioWait(MarioActor*);

    u8 _11[7];
};

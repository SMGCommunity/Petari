#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFreeze : public MarioState {
public:
    MarioFreeze(MarioActor*);

    u8 _11[15];
};

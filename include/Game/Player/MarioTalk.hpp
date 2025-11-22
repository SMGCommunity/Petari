#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioTalk : public MarioState {
public:
    MarioTalk(MarioActor*);

    u8 _11[11];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioRecovery : public MarioState {
public:
    MarioRecovery(MarioActor*);

    u8 _11[0x7B];
};

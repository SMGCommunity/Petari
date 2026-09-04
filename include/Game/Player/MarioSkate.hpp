#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSkate : public MarioState {
public:
    MarioSkate(MarioActor*);

    void exitJump();

    u8 _11[0x17];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSkate : public MarioState {
public:
    MarioSkate(MarioActor*);

    u8 _11[0x17];
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBlown : public MarioState {
public:
    MarioBlown(MarioActor*);
    u8 _11[0x17];
};

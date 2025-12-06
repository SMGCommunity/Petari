#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioAbyssDamage : public MarioState {
public:
    MarioAbyssDamage(MarioActor*);
    u8 _11[0x13];
};

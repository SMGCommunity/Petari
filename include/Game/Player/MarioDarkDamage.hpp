#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioDarkDamage : public MarioState {
public:
    MarioDarkDamage(MarioActor*);
    u8 _11[7];
};

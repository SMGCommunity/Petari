#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioCrush : public MarioState {
public:
    MarioCrush(MarioActor*);
    u8 _11[7];
};

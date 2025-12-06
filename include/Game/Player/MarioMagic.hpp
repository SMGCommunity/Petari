#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioMagic : public MarioState {
public:
    MarioMagic(MarioActor*);

    u8 _11[3];
};

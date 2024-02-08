#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStun : public MarioState {
public:
    MarioStun(MarioActor *);
    u8 _11[7];
};

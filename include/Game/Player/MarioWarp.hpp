#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWarp : public MarioState {
public:
    MarioWarp(MarioActor*);

    u8 _11[0x7F];
};

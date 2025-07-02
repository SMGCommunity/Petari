#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFlip : public MarioState {
public:
    MarioFlip(MarioActor *);
    u8 _11; // FAKE
    u16 _12; // FAKE
    u16 _14;
};

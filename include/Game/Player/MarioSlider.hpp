#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSlider : public MarioState {
public:
    MarioSlider(MarioActor *);
    u8 _11[0x33];
};

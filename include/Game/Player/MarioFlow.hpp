#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFlow : public MarioState {
public:
    MarioFlow(MarioActor*);
    u16 _12;
    u16 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
};

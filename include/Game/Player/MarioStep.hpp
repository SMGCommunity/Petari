#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStep : public MarioState {
public:
    MarioStep(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();

    f32 _14;
    f32 _18;
};

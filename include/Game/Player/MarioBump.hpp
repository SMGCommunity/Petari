#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioBump : public MarioState {
public:
    MarioBump(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();

    /* 0x12 */ u16 _12;
    /* 0x14 */ f32 _14;
    /* 0x18 */ TVec3f _18;
};

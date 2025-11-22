#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioDamage : public MarioState {
public:
    MarioDamage(MarioActor*);

    u8 _11;
    u8 _12;
    u16 _14;
    u16 _16;
    u16 _18;
    TVec3f _1C;
    u32 _28;
    u32 _2C;
};

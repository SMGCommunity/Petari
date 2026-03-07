#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWait : public MarioState {
public:
    MarioWait(MarioActor*);

    /* 0x14 */ u16 _14;
    /* 0x16 */ u16 _16;
    /* 0x18 */ u8 _18[3];
};

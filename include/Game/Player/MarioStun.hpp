#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStun : public MarioState {
public:
    MarioStun(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();

    u8 _11;
    u16 _12;
    u16 _14;
    u16 _16;
};

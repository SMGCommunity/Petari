#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioMagic : public MarioState {
public:
    MarioMagic(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x18 */ virtual bool close();
    /* 0x1C */ virtual bool update();

    /* 0x12 */ u16 _12;
};

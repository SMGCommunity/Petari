#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFlow : public MarioState {
public:
    MarioFlow(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
    /* 0x18 */ TVec3f _18;
};

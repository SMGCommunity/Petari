#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFireRun : public MarioState {
public:
    MarioFireRun(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();

    bool move();

    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 _14;
    /* 0x18 */ f32 _18;
};

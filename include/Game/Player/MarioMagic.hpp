#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioMagic : public MarioState {
public:
    MarioMagic(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    /* 0x12 */ u16 _12;
};

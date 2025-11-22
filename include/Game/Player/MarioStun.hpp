#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStun : public MarioState {
public:
    MarioStun(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    u8 _11;
    u16 _12;
    u16 _14;
    u16 _16;
};

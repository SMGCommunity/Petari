#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFlow : public MarioState {
public:
    MarioFlow(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    u16 _12;
    u16 _14;
    TVec3f _18;
};

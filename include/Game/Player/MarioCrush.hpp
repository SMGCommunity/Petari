#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioCrush : public MarioState {
public:
    MarioCrush(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    //padding
    u16 _12;
    u16 _14;
};

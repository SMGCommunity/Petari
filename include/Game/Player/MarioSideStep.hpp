#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSideStep : public MarioState {
public:
    MarioSideStep(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
};

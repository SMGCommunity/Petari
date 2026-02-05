#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioStep : public MarioState {
public:
    MarioStep(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();

    f32 _14;
    f32 _18;
};

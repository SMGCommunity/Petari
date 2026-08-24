#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFrontStep : public MarioState {
public:
    MarioFrontStep(MarioActor*);

    virtual bool start();
    virtual bool update();
    virtual bool close();
    virtual bool postureCtrl(MtxPtr);
};

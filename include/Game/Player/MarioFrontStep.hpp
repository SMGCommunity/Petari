#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFrontStep : public MarioState {
public:
    MarioFrontStep(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool postureCtrl(MtxPtr);

    u8 _11[3];
};

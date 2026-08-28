#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFrontStep : public MarioState {
public:
    MarioFrontStep(MarioActor*);

    /* 0x14 */ virtual bool start();
    /* 0x1C */ virtual bool update();
    /* 0x18 */ virtual bool close();
    /* 0x28 */ virtual bool postureCtrl(MtxPtr);
};

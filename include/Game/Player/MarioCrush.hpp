#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioCrush : public MarioState {
public:
    MarioCrush(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    
    /* 0x12 */ u16 _12;
    /* 0x14 */ u16 mTimer;
};

#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFpView : public MarioState {
public:
    MarioFpView(MarioActor *);
    
    void forceClose();

    u8 _11[11];
};

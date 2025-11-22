#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFpView : public MarioState {
public:
    MarioFpView(MarioActor*);

    u8 _11[11];
};

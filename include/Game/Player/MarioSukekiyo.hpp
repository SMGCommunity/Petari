#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSukekiyo : public MarioState {
public:
    MarioSukekiyo(MarioActor*);

    u8 _11[0x3B];
};

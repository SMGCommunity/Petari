#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioParalyze : public MarioState {
public:
    MarioParalyze(MarioActor*);

    u8 _11[11];
};

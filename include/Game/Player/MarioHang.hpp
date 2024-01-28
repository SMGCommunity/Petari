#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioHang : public MarioState {
public:
    MarioHang(MarioActor *);
    u8 _11[0x33];
};

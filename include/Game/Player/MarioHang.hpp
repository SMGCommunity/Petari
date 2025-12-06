#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioHang : public MarioState {
public:
    MarioHang(MarioActor*);
    u8 _11[0x6];
    u16 mHangTimer;  // 0x18
    u8 _1A[0x2A];
};

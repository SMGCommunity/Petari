#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;
class HitSensor;
class Triangle;

class MarioHang : public MarioState {
public:
    MarioHang(MarioActor*);

    u16 _12;
    u16 _14;
    u16 _16;
    u16 mHangTimer;
    u8 _1A;
    u8 _1B;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    u8 _1F;
    Triangle* _20;
    Triangle* _24;
    TVec3f _28;
    TVec3f _34;
    HitSensor* mWallSensor;
};

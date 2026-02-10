#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWall : public MarioState {
public:
    MarioWall(MarioActor*);
    bool startJump();
    u32 _14;
    u32 _18;
    u8 _1C;
    u8 _1D;
    u8 _1E;
    f32 _20;
    TVec3f _24;
    TVec3f _30;
};

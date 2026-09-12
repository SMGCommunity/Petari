#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioWall : public MarioState {
public:
    MarioWall(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();

    void initTriangleJump();
    u8 isCancel();
    bool startJump();
    bool startBackJump(u32);

    /* 0x14 */ u32 _14;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u8 _1C;
    /* 0x1D */ u8 _1D;
    /* 0x1E */ u8 _1E;
    /* 0x20 */ f32 _20;
    /* 0x24 */ TVec3f _24;
    /* 0x30 */ TVec3f _30;
};

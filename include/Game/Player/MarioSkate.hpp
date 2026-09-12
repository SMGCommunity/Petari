#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioSkate : public MarioState {
public:
    MarioSkate(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice() {
        return false;
    }
    virtual bool postureCtrl(MtxPtr);

    void exitJump();

    /* 0x14 */ u32 _14;
    /* 0x18 */ u8 _18;
    /* 0x19 */ bool _19;
    /* 0x1A */ u8 _1A;
    /* 0x1B */ s8 _1B;
    /* 0x1C */ u8 _1C;
    /* 0x1D */ u8 _1D;
    /* 0x20 */ f32 _20;
    /* 0x24 */ f32 _24;
};

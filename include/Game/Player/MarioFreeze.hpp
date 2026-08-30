#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioFreeze : public MarioState {
public:
    MarioFreeze(MarioActor*);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();

    /* 0x11 */ bool mIsFrozen;
    /* 0x14 */ f32 _14;
    /* 0x18 */ u16 _18;
    /* 0x1A */ u16 mFreezeTimer;
    /* 0x1C */ u16 _1C;
};

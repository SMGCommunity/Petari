#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioTalk : public MarioState {
public:
    MarioTalk(MarioActor*);

    virtual bool update();
    virtual bool notice();
    virtual bool close();
    virtual bool start();

    /* 0x12 */ u16 _12;
    /* 0x14 */ const LiveActor* _14;
    /* 0x18 */ bool mIsUpdate;
};

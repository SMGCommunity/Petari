#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioTalk : public MarioState {
public:
    MarioTalk(MarioActor*);

    /* 0x1C */ virtual bool update();
    /* 0x20 */ virtual bool notice();
    /* 0x18 */ virtual bool close();
    /* 0x14 */ virtual bool start();

    /* 0x12 */ u16 _12;
    /* 0x14 */ const LiveActor* _14;
    /* 0x18 */ bool mIsUpdate;
};

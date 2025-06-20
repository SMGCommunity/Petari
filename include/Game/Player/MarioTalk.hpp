#pragma once

#include "Game/Player/MarioState.hpp"

class MarioActor;

class MarioTalk : public MarioState {
public:
    MarioTalk(MarioActor *);

    virtual bool start();
    virtual bool close();
    virtual bool update();
    virtual bool notice();

    // padding
    const LiveActor* mTalkActor;  // 0x14
    bool _18;
};

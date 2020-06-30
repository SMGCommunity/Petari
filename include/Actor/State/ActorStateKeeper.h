#pragma once

#include "Actor/Nerve/Nerve.h"
#include "Actor/State/ActorStateBase.h"

class ActorStateKeeper
{
public:
    ActorStateKeeper(s32);

    void addState(ActorStateBaseInterface *, const Nerve *, const char *);
    bool updateCurrentState();
    void startState(const Nerve *);
    void endState(const Nerve *);

    s32 mStateCount; // _0
    u32 _4;
    u32 _8;
    u32 _C;
};
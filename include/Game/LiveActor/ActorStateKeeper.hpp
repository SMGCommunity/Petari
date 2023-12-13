#pragma once

#include "Game/LiveActor/Nerve.hpp"
#include "Game/LiveActor/ActorStateBase.hpp"

class ActorStateKeeper {
public:
    ActorStateKeeper(int);

    void addState(ActorStateBaseInterface *, const Nerve *, const char *);
    bool updateCurrentState();
    void startState(const Nerve *);
    void endState(const Nerve *);
    Spine* findStateInfo(const Nerve *);

    s32 _0;
    u32 _4;
    ActorStateBaseInterface* mNerves;           // _8
    Spine* mCurrentState;                       // _C
};
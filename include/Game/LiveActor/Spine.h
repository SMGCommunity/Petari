#pragma once

#include "Game/LiveActor/Nerve.h"
#include <revolution.h>

class ActorStateKeeper;

class Spine
{
public:
    Spine(void *, const Nerve *);

    void update();
    void setNerve(const Nerve *);
    const Nerve* getCurrentNerve() const;
    void changeNerve();
    void initStateKeeper(s32);

    void* mExecutor; // _0
    const Nerve* mCurNerve; // _4
    const Nerve* mNextNerve; // _8
    u32 mStep; // _C
    ActorStateKeeper* mStateKeeper; // _10
};
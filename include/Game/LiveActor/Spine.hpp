#pragma once

#include <revolution.h>

class ActorStateKeeper;
class Nerve;

class Spine
{
public:
    Spine(void *, const Nerve *);

    void update();
    void setNerve(const Nerve *);
    const Nerve* getCurrentNerve() const;
    void changeNerve();
    void initStateKeeper(int);

    void* mExecutor; // 0x0
    const Nerve* mCurNerve; // 0x4
    const Nerve* mNextNerve; // 0x8
    s32 mStep; // 0xC
    ActorStateKeeper* mStateKeeper; // 0x10
};
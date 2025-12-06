#pragma once

#include <revolution/types.h>

class ActorStateKeeper;
class Nerve;

class Spine {
public:
    Spine(void*, const Nerve*);

    void update();
    void setNerve(const Nerve*);
    const Nerve* getCurrentNerve() const;
    void changeNerve();
    void initStateKeeper(int);

    /* 0x00 */ void* mExecutor;
    /* 0x04 */ const Nerve* mCurrNerve;
    /* 0x08 */ const Nerve* mNextNerve;
    /* 0x0C */ s32 mStep;
    /* 0x10 */ ActorStateKeeper* mStateKeeper;
};

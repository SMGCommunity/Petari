#pragma once

#include <revolution/types.h>

class LiveActor;

class ActiveActorList {
public:
    ActiveActorList(int);

    bool hasTooMany() const {
        return (mCurCount >= mMaxCount);
    }

    bool isFull() const;
    void addActor(LiveActor*);
    void removeDeadActor();
    void clear();
    void killAll();

    /* 0x0 */ LiveActor** mActorList;
    /* 0x4 */ s32 mCurCount;
    /* 0x8 */ int mMaxCount;
};
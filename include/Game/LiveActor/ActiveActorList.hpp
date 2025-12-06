#pragma once

#include "Game/Util.hpp"
#include <revolution.h>

class LiveActor;

class ActiveActorList {
public:
    ActiveActorList(int);

    bool hasTooMany() const { return (mCurCount >= mMaxCount); }

    bool isFull() const;
    void addActor(LiveActor*);
    void removeDeadActor();
    void clear();
    void killAll();

    LiveActor** mActorList;  // 0x0
    s32 mCurCount;           // 0x4
    int mMaxCount;           // 0x8
};
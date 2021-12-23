#pragma once

#include "Game/Util.h"
#include <revolution.h>

class LiveActor;

class ActiveActorList {
public:
    ActiveActorList(int);

    bool isFull() const;
    void addActor(LiveActor *);
    void removeDeadActor();
    void clear();
    void killAll();

    LiveActor** mActorList; // _0
    s32 mCurCount;          // _4
    int mMaxCount;          // _8
};
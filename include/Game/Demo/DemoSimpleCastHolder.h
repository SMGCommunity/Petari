#pragma once

#include "Game/Util.h"

class LayoutActor;
class LiveActor;
class NameObj;

class DemoSimpleCastHolder {
public:
    DemoSimpleCastHolder(s32, s32, s32);

    void registerActor(LiveActor *);
    void registerActor(LayoutActor *);
    void registerNameObj(NameObj *);
    void movementOnAllCasts();

    MR::AssignableArray<LiveActor *> mLiveActors;       // _0
    s32 mMaxLiveActorCount;                             // _8
    MR::AssignableArray<LayoutActor *> mLayoutActors;   // _C
    s32 mMaxLayoutActorCount;                           // _14
    MR::AssignableArray<NameObj *>  mNameObjs;          // _18
    s32 mMaxNameObjCount;                               // _20
};
#pragma once

#include "Game/Util.hpp"

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

    MR::AssignableArray<LiveActor *> mLiveActors;       // 0x0
    s32 mMaxLiveActorCount;                             // 0x8
    MR::AssignableArray<LayoutActor *> mLayoutActors;   // 0xC
    s32 mMaxLayoutActorCount;                           // 0x14
    MR::AssignableArray<NameObj *>  mNameObjs;          // 0x18
    s32 mMaxNameObjCount;                               // 0x20
};
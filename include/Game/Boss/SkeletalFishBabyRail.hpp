#pragma once

#include "Game/LiveActor//LiveActor.hpp"

class SkeletalFishBabyRailHolder;

class SkeletalFishBabyRail : public LiveActor {
public:
    SkeletalFishBabyRail(const char*);

    /* 0x08 */ virtual ~SkeletalFishBabyRail();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    s32 _8C;
};
#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBossRail;

class SkeletalFishBossRailHolder : public NameObj {
public:

    void add(SkeletalFishBossRail *);

    s32 mNumRails;                          // _C
    SkeletalFishBossRail* mRails[3];        // _10
};

namespace MR {
    SkeletalFishBossRailHolder* getSkeletalFishBossRailHolder();
    void createSkeletalFishBossRailHolder();
};
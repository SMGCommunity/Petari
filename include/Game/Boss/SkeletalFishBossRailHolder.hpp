#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBossRail;

class SkeletalFishBossRailHolder : public NameObj {
public:
    SkeletalFishBossRailHolder(const char *);

    virtual ~SkeletalFishBossRailHolder();

    void add(SkeletalFishBossRail *);
    SkeletalFishBossRail* getInOrder(s32) const;
    SkeletalFishBossRail* getByID(s32) const;

    s32 mNumRails;                          // _C
    SkeletalFishBossRail* mRails[3];        // _10
};

namespace MR {
    SkeletalFishBossRailHolder* getSkeletalFishBossRailHolder();
    void createSkeletalFishBossRailHolder();
};
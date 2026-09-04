#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBossRail;

class SkeletalFishBossRailHolder : public NameObj {
public:
    SkeletalFishBossRailHolder(const char*);

    void add(SkeletalFishBossRail*);
    SkeletalFishBossRail* getInOrder(s32) const;
    SkeletalFishBossRail* getByID(s32) const;

    /* 0x0C */ s32 mNumRails;
    /* 0x10 */ SkeletalFishBossRail* mRails[3];
};

namespace MR {
    SkeletalFishBossRailHolder* getSkeletalFishBossRailHolder();
    void createSkeletalFishBossRailHolder();
};  // namespace MR

#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBabyRail;

class SkeletalFishBabyRailHolder : public NameObj {
public:
    SkeletalFishBabyRailHolder(const char *);

    virtual void initAfterPlacement();

    void add(SkeletalFishBabyRail *);
};

namespace MR {
    void createSkeletalFishBabyRailHolder();
    SkeletalFishBabyRailHolder* getSkeletalFishBabyRailHolder();
};
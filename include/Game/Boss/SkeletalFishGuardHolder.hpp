#pragma once

#include "Game/NameObj/NameObj.hpp"

class SkeletalFishBoss;
class SkeletalFishGuard;
class SkeletalFishBossRail;

class SkeletalFishGuardHolder : public NameObj {
public:
    SkeletalFishGuardHolder(SkeletalFishBoss*, s32, const char*);

    virtual ~SkeletalFishGuardHolder();
    virtual void movement();

    void forceAppearAll();
    void forceKillAll();
    void killAll();
    void notifyAttack(SkeletalFishGuard*);
    void invalidate();
    void validate();
    void checkRailChangedAndReset();
    s32 countAppearableGuard() const;
    s32 countGuardAlive() const;
    void checkDistanceAndAppear(s32);
    void appearGuard(s32, s32);
    void createAndInitGuard();

    /* 0x0C */ SkeletalFishBoss* mFishBoss;
    /* 0x10 */ SkeletalFishBossRail* mBossRail;
    /* 0x14 */ s32 mNumGuards;
    /* 0x18 */ SkeletalFishGuard** mGuards;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ u8 _20;
};

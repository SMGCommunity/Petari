#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry.hpp>

class SkeletalFishBoss;

class SkeletalFishBossInfo : public NameObj {
public:
    class GuardStatus {
    public:
        GuardStatus();

        TVec3f mGuardPosLevel;  // 0x0
        s32 mGuardWaitLevelID;  // 0xC
    };

    struct LevelStatus {
    public:
        inline GuardStatus* getGuardStatus(u32 idx) const { return &mStatusArray[idx]; }

        s32 mEnergyLevel;                                 // 0x0
        f32 mSpeedLevel;                                  // 0x4
        s32 mRailIDLevel;                                 // 0x8
        f32 mGuardOffsLevel;                              // 0xC
        s32 mGuardAppearNumLevel;                         // 0x10
        SkeletalFishBossInfo::GuardStatus* mStatusArray;  // 0x14
    };

    SkeletalFishBossInfo(SkeletalFishBoss*, s32, s32, const char*);

    virtual ~SkeletalFishBossInfo();
    virtual void init(const JMapInfoIter&);

    LevelStatus* getLevelStatus(s32) const;
    void createLevelStatus();
    void loadLevelStatus(const JMapInfoIter&);
    void loadGuardStatus(const JMapInfoIter&, s32, s32);

    SkeletalFishBoss* mFishBoss;     // 0xC
    s32 mLevelNum;                   // 0x10
    s32 mGuardNum;                   // 0x14
    LevelStatus* mLevelStatusArray;  // 0x18
};
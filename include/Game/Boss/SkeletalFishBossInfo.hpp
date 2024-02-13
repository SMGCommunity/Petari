#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry.hpp>

class SkeletalFishBoss;

class SkeletalFishBossInfo : public NameObj {
public:

    class GuardStatus {
    public:
        GuardStatus();

        TVec3f mGuardPosLevel;      // _0
        s32 mGuardWaitLevelID;      // _C
    };

    struct LevelStatus {
    public:
        inline GuardStatus* getGuardStatus(u32 idx) const {
            return &mStatusArray[idx];
        }

        s32 mEnergyLevel;                                   // _0
        f32 mSpeedLevel;                                    // _4
        s32 mRailIDLevel;                                   // _8
        f32 mGuardOffsLevel;                                // _C
        s32 mGuardAppearNumLevel;                           // _10
        SkeletalFishBossInfo::GuardStatus* mStatusArray;    // _14
    };

    SkeletalFishBossInfo(SkeletalFishBoss *, s32, s32, const char *);

    virtual ~SkeletalFishBossInfo();
    virtual void init(const JMapInfoIter &);

    LevelStatus* getLevelStatus(s32) const;
    void createLevelStatus();
    void loadLevelStatus(const JMapInfoIter &);
    void loadGuardStatus(const JMapInfoIter &, s32, s32);

    SkeletalFishBoss* mFishBoss;                        // _C
    s32 mLevelNum;                                      // _10
    s32 mGuardNum;                                      // _14
    LevelStatus* mLevelStatusArray;                     // _18
};
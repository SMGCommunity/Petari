#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class SkeletalFishBoss;

class SkeletalFishBossInfo : public NameObj {
public:
    class GuardStatus {
    public:
        GuardStatus();

        /* 0x00 */ TVec3f mGuardPosLevel;
        /* 0x0C */ s32 mGuardWaitLevelID;
    };

    struct LevelStatus {
    public:
        inline GuardStatus* getGuardStatus(u32 idx) const {
            return &mStatusArray[idx];
        }

        /* 0x00 */ s32 mEnergyLevel;
        /* 0x04 */ f32 mSpeedLevel;
        /* 0x08 */ s32 mRailIDLevel;
        /* 0x0C */ f32 mGuardOffsLevel;
        /* 0x10 */ s32 mGuardAppearNumLevel;
        /* 0x14 */ SkeletalFishBossInfo::GuardStatus* mStatusArray;
    };

    SkeletalFishBossInfo(SkeletalFishBoss*, s32, s32, const char*);

    virtual void init(const JMapInfoIter&);

    LevelStatus* getLevelStatus(s32) const;
    void createLevelStatus();
    void loadLevelStatus(const JMapInfoIter&);
    void loadGuardStatus(const JMapInfoIter&, s32, s32);

    /* 0x0C */ SkeletalFishBoss* mFishBoss;
    /* 0x10 */ s32 mLevelNum;
    /* 0x14 */ s32 mGuardNum;
    /* 0x18 */ LevelStatus* mLevelStatusArray;
};

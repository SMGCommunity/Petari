#pragma once

#include "JSystem/JGeometry/TVec.hpp"

struct PowerStarInfo {
    bool _0;
    bool _1;
    bool _2;
    bool _3;
    const char* mDemoName;
};

class PowerStarEventKeeper {
public:
    PowerStarEventKeeper();

    void initStarInfoTable();
    s32 findStarID(const char*) const;
    void declareStar(const char*, const char*, s32, bool);
    bool isGreen(const char*, s32) const;
    bool isRed(const char*, s32) const;
    bool isGrand(const char*, s32) const;
    void initStarInfoTableAfterPlacement();
    void requestAppearPowerStar(const char*, s32, const TVec3f*, bool, bool);
    bool isExistPowerStarAtScenario(int) const;
    bool isCorrectStarIdWithBBS(const char*, const char*, s32) const NO_INLINE;
    bool isSuccess(const char*, s32) const;

    inline PowerStarInfo* getStarInfo(u32 idx) const {
        return &mInfoTbl[idx];
    }

    inline s32 calcStarID(u32 scenarioNo) const {
        return (1 << (scenarioNo - 1));
    }

    /* 0x0 */ u32 mStartScenario;
    /* 0x4 */ s32 mPowerStarNum;
    /* 0x8 */ PowerStarInfo* mInfoTbl;
};

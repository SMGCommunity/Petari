#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

struct PowerStarInfo {
    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    const char* _4;
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
    bool isCorrectStarIdWithBBS(const char*, const char*, s32) const;
    bool isSuccess(const char*, s32) const;

    u32 _0;
    s32 mPowerStarNum;         // 0x4
    PowerStarInfo** mInfoTbl;  // 0x8
};
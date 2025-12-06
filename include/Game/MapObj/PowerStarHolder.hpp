#pragma once

#include "Game/MapObj/PowerStar.hpp"

struct PowerStarRequestInfo {
    PowerStar* mStar;  // 0x0
    int mStarNum;      // 0x4
    bool mIsAppear;    // 0x8
};

class PowerStarHolder : public NameObj {
public:
    PowerStarHolder(const char*);

    virtual ~PowerStarHolder();
    virtual void init(const JMapInfoIter&);

    void registerPowerStar(PowerStar*, int);
    void requestAppearPowerStar(int, const TVec3f*, bool);
    void appearPowerStarWithoutDemo(int);
    PowerStar* getAppearedPowerStar(int) const;
    PowerStarRequestInfo* findPowerStarRequestInfo(int) const;

    PowerStarRequestInfo* mInfos[0x10];  // 0xC
    s32 mNumInfos;                       // 0x4C
};

namespace MR {
    void registerPowerStar(PowerStar*, int);
};

class PowerStarFunction {
public:
    static bool isEndPowerStarAppearDemo(int);
    static PowerStar* findPowerStar(int);
};
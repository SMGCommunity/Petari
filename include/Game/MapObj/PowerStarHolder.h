#pragma once

#include "Game/MapObj/PowerStar.h"

struct PowerStarRequestInfo {
    PowerStar* mStar;       // _0
    int mStarNum;           // _4
    bool mIsAppear;         // _8
};

class PowerStarHolder : public NameObj {
public:
    PowerStarHolder(const char *);

    virtual ~PowerStarHolder();
    virtual void init(const JMapInfoIter &);

    void registerPowerStar(PowerStar *, int);
    void requestAppearPowerStar(int, const TVec3f *, bool);
    void appearPowerStarWithoutDemo(int);
    PowerStar* getAppearedPowerStar(int) const;
    PowerStarRequestInfo* findPowerStarRequestInfo(int) const;

    PowerStarRequestInfo* mInfos[0x10];     // _C
    s32 mNumInfos;                          // _4C
};

namespace MR {
    void registerPowerStar(PowerStar *, int);
};

class PowerStarFunction {
public:
    static bool isEndPowerStarAppearDemo(int);
    static PowerStar* findPowerStar(int);
};
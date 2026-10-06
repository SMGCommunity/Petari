#pragma once

#include "Game/MapObj/PowerStar.hpp"

struct PowerStarRequestInfo {
    /* 0x0 */ PowerStar* mStar;
    /* 0x4 */ int mStarNum;
    /* 0x8 */ bool mIsAppear;
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

    /* 0x0C */ PowerStarRequestInfo* mInfos[0x10];
    /* 0x4C */ s32 mNumInfos;
};

namespace MR {
    void registerPowerStar(PowerStar*, int);
};  // namespace MR

class PowerStarFunction {
public:
    static bool isEndPowerStarAppearDemo(int);
    static PowerStar* findPowerStar(int);
};

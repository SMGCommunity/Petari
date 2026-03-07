#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class LightInfo {
public:
    _GXColor mColor;       // 0x0
    TVec3f mPos;           // 0x4
    bool mIsFollowCamera;  // 0x10
};

class LightInfoCoin : public LightInfo {
public:
    inline LightInfoCoin() {}

    u8 _14;
    u8 _15;
    u8 _16;
    u8 _17;
    f32 _18;
};

class ActorLightInfo {
public:
    inline ActorLightInfo() {}

    void operator=(const ActorLightInfo&);

    LightInfo mInfo0;  // 0x0
    LightInfo mInfo1;  // 0x14
    u8 mAlpha2;        // 0x28
    _GXColor mColor;   // 0x29
};

struct AreaLightInfo {
    const char* mAreaLightName;   // 0x0
    s32 mInterpolate;             // 0x4
    bool mFix;                    // 0x8
    ActorLightInfo mPlayerLight;  // 0xC
    ActorLightInfo mStrongLight;  // 0x3C
    ActorLightInfo mWeakLight;    // 0x6C
    ActorLightInfo mPlanetLight;  // 0x9C
};

class LightDataHolder {
public:
    LightDataHolder();

    void initLightData();

    AreaLightInfo* findAreaLight(const char*) const;
    const char* getDefaultAreaLightName() const;
    s32 getDefaultStepInterpolate() const;

    s32 mLightCount;         // 0x0
    AreaLightInfo* mLights;  // 0x4
    LightInfoCoin _8;
};

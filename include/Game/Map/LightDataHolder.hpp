#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution.h>

class LightInfo {
public:
    /* 0x00 */ GXColor mColor;
    /* 0x04 */ TVec3f mPos;
    /* 0x10 */ bool mIsFollowCamera;
};

class LightInfoCoin : public LightInfo {
public:
    LightInfoCoin() {
    }

    u8 _14;
    u8 _15;
    u8 _16;
    u8 _17;
    f32 _18;
};

class ActorLightInfo {
public:
    ActorLightInfo() {
    }

    void operator=(const ActorLightInfo&);

    LightInfo mInfo0;  // 0x0
    LightInfo mInfo1;  // 0x14
    u8 mAlpha2;        // 0x28
    GXColor mColor;   // 0x29
};

struct AreaLightInfo {
    /* 0x00 */ const char* mAreaLightName;
    /* 0x04 */ s32 mInterpolate;
    /* 0x08 */ bool mFix;
    /* 0x0C */ ActorLightInfo mPlayerLight;
    /* 0x3C */ ActorLightInfo mStrongLight;
    /* 0x6C */ ActorLightInfo mWeakLight;
    /* 0x9C */ ActorLightInfo mPlanetLight;
};

class LightDataHolder {
public:
    LightDataHolder();

    void initLightData();

    AreaLightInfo* findAreaLight(const char*) const;
    const char* getDefaultAreaLightName() const;
    s32 getDefaultStepInterpolate() const;

    /* 0x00 */ s32 mLightCount;
    /* 0x04 */ AreaLightInfo* mLights;
    /* 0x08 */ LightInfoCoin _8;
};

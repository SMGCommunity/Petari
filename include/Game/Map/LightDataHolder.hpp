#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.hpp"

class LightInfo {
public:
    _GXColor mColor;    // _0
    TVec3f mPos;        // _4
    u8 mIsFollowCamera; // _10
};

class LightInfoCoin : public LightInfo {
public:
    inline LightInfoCoin() {

    }

    u8 _14;
    u8 _15;
    u8 _16;
    u8 _17;
    f32 _18;
};

class ActorLightInfo {
public:
    inline ActorLightInfo() {
        
    }

    void operator=(const ActorLightInfo &);

    LightInfo mInfo0;   // _0
    LightInfo mInfo1;   // _14
    u8 mAlpha2;         // _28
    _GXColor mColor;    // _29
};

struct AreaLightInfo {
    const char* mAreaLightName;     // _0
    s32 mInterpolate;               // _4
    bool mFix;                      // _8
    ActorLightInfo mPlayerLight;    // _C
    ActorLightInfo mStrongLight;    // _3C
    ActorLightInfo mWeakLight;      // _6C
    ActorLightInfo mPlanetLight;    // _9C
};

namespace {
    static const char* sDefaultAreaLightName = "デフォルト";
};

class LightDataHolder {
public:
    LightDataHolder();

    void initLightData();

    AreaLightInfo* findAreaLight(const char *) const;
    const char* getDefaultAreaLightName() const;
    s32 getDefaultStepInterpolate() const;

    s32 mLightCount;            // _0
    AreaLightInfo* mLights;    // _4
    LightInfoCoin _8;
};
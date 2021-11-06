#pragma once

#include "Game/AreaObj/LightArea.h"

#include <revolution.h>

class ZoneLightID {
public:
    ZoneLightID();

    void clear();
    bool isTargetArea(const LightArea *) const;
    bool isOutOfArea() const;

    s32 _0;
    s32 mLightID;   // _4
};

// I am assuming they called this "AreaInfo" because the debug map
// tells me there was a function contained in LightZoneInfo called "getAreaInfo"
struct AreaInfo {
    s32 mID;                        // _0
    const char* mAreaLightName;     // _4
};

class LightZoneInfo {
public:
    LightZoneInfo();

    void init(s32);

    const char* getAreaLightNameInZoneData(s32) const;

    u32 mAreaCount;          // _0
    AreaInfo* mAreaInfo;   // _4
};

class LightZoneDataHolder {
public:
    LightZoneDataHolder();

    void initZoneData();
    const char* getAreaLightNameInZoneData(const ZoneLightID &) const;
    const char* getDefaultStageAreaLightName() const;

    u32 mCount;                 // _0
    LightZoneInfo* mZoneInfo;  // _4
};
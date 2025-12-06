#pragma once

#include <revolution.h>

class LightArea;

class ZoneLightID {
public:
    ZoneLightID();

    void clear();
    bool isTargetArea(const LightArea*) const;
    bool isOutOfArea() const;

    s32 _0;
    s32 mLightID;  // 0x4
};

// I am assuming they called this "AreaInfo" because the debug map
// tells me there was a function contained in LightZoneInfo called "getAreaInfo"
struct AreaInfo {
    s32 mID;                     // 0x0
    const char* mAreaLightName;  // 0x4
};

class LightZoneInfo {
public:
    LightZoneInfo();

    void init(s32);

    const char* getAreaLightNameInZoneData(s32) const;

    s32 mAreaCount;       // 0x0
    AreaInfo* mAreaInfo;  // 0x4
};

class LightZoneDataHolder {
public:
    LightZoneDataHolder();

    void initZoneData();
    const char* getAreaLightNameInZoneData(const ZoneLightID&) const;
    const char* getDefaultStageAreaLightName() const;

    s32 mCount;                // 0x0
    LightZoneInfo* mZoneInfo;  // 0x4
};
#pragma once

#include <revolution.h>

class LightArea;

class ZoneLightID {
public:
    ZoneLightID();

    void clear();
    bool isTargetArea(const LightArea*) const;
    bool isOutOfArea() const;

    /* 0x0 */ s32 _0;
    /* 0x4 */ s32 mLightID;
};

// I am assuming they called this "AreaInfo" because the debug map
// tells me there was a function contained in LightZoneInfo called "getAreaInfo"
struct AreaInfo {
    /* 0x0 */ s32 mID;
    /* 0x4 */ const char* mAreaLightName;
};

class LightZoneInfo {
public:
    LightZoneInfo();

    void init(s32);

    const char* getAreaLightNameInZoneData(s32) const;

    /* 0x0 */ s32 mAreaCount;
    /* 0x4 */ AreaInfo* mAreaInfo;
};

class LightZoneDataHolder {
public:
    LightZoneDataHolder();

    void initZoneData();
    const char* getAreaLightNameInZoneData(const ZoneLightID&) const;
    const char* getDefaultStageAreaLightName() const;

    /* 0x0 */ s32 mCount;
    /* 0x4 */ LightZoneInfo* mZoneInfo;
};
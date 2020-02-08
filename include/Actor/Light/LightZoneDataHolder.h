#ifndef LIGHTZONEDATAHOLDER_H
#define LIGHTZONEDATAHOLDER_H

#include <revolution.h>

class LightArea;

class ZoneLightID
{
public:
    ZoneLightID();

    void clear();
    bool isTargetArea(const LightArea *) const;
    bool isOutOfArea() const;

    s32 mZoneID; // _0
    s32 _4; // this is Obj_arg0 of LightArea
};

class LightZoneInfo
{
public:
    LightZoneInfo();

    const char* getAreaLightNameInZoneData(s64) const;

    u32 _0;
    u32 _4;
};

#endif // LIGHTZONEDATAHOLDER_H
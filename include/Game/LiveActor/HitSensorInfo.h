#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

class HitSensor;

class HitSensorInfo
{
public:
    HitSensorInfo(const char *, HitSensor *, const TVec3f *, Mtx *, const TVec3f &, bool);

    void update();
    void doObjCol();

    const char* mName; // _0
    s32 mHashCode; // _4
    HitSensor* mSensor; // _8
    TVec3f _C;
    TVec3f* _18;
    Mtx* _1C;
    bool _20;
    u8 _21; // _Padding
    u8 _22;
    u8 _23;
};
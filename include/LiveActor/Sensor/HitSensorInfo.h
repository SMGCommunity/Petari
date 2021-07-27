#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution.h>

class HitSensor;

class HitSensorInfo
{
public:
    HitSensorInfo(const char *, HitSensor *, const JGeometry::TVec3f *, Mtx *, const JGeometry::TVec3f &, bool);

    void update();
    void doObjCol();

    const char* mName; // _0
    s32 mHashCode; // _4
    HitSensor* mSensor; // _8
    JGeometry::TVec3f _C;
    JGeometry::TVec3f* _18;
    Mtx* _1C;
    bool _20;
};
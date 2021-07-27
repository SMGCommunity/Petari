#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution.h>

class HitSensor;

class HitSensorInfo
{
public:
    HitSensorInfo(const char *, HitSensor *, const JGeometry::TVec3<f32> *, Mtx *, const JGeometry::TVec3<f32> &, bool);

    void update();
    void doObjCol();

    const char* mName; // _0
    s32 mHashCode; // _4
    HitSensor* mSensor; // _8
    JGeometry::TVec3<f32> _C;
    JGeometry::TVec3<f32>* _18;
    Mtx* _1C;
    bool _20;
};
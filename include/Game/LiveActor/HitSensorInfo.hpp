#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class HitSensor;

class HitSensorInfo
{
public:
    HitSensorInfo(const char *, HitSensor *, const TVec3f *, MtxPtr, const TVec3f &, bool);

    void update();
    void doObjCol();

    const char* mName; // _0
    s32 mHashCode; // _4
    HitSensor* mSensor; // _8
    TVec3f _C;
    const TVec3f* _18;
    MtxPtr _1C;
    bool _20;
    u8 _21; // _Padding
    u8 _22;
    u8 _23;
};
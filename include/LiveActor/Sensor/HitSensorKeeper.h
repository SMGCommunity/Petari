#pragma once

#include "LiveActor/Sensor/HitSensor.h"

class HitSensorKeeper
{
public:
    HitSensorKeeper(s32);

    void add(const char *, u32, u16, f32, LiveActor *, const JGeometry::TVec3<f32> &);
    void addPos(const char *, u32, u16, f32, LiveActor *, JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);

    HitSensor* getSensor(const char *) const;
    void update();
    void doObjCol();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();

    u32 _0;
    s32 mNumSensors; // _4
    HitSensor** mSensors; // _8
    u32 _C;
    u32 _10;
};
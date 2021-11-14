#pragma once

#include "Game/LiveActor/HitSensor.h"

class HitSensorInfo;

class HitSensorKeeper {
public:
    HitSensorKeeper(int);

    HitSensor* add(const char *, u32, u16, f32, LiveActor *, const TVec3f &);
    HitSensor* addPos(const char *, u32, u16, f32, LiveActor *, TVec3f *, const TVec3f &);

    HitSensor* getSensor(const char *) const;
    void update();
    void doObjCol();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();

    void registHitSensorInfo(HitSensorInfo *);

    u32 _0;
    s32 mSensorCount; // _4
    HitSensor** mSensors; // _8
    u32 _C;
    u32 _10;
};
#pragma once

#include "Game/LiveActor/HitSensor.h"

class HitSensorInfo;

class HitSensorKeeper {
public:
    HitSensorKeeper(int);

    HitSensor* add(const char *, u32, u16, f32, LiveActor *, const TVec3f &);
    HitSensor* addPos(const char *, u32, u16, f32, LiveActor *, const TVec3f *, const TVec3f &);
    HitSensor* addMtx(const char *, u32, u16, f32, LiveActor *, Mtx, const TVec3f &);
    HitSensor* addCallback(const char *, u32, u16, f32, LiveActor *);

    HitSensor* getSensor(const char *) const;
    void update();
    void doObjCol();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    HitSensorInfo* getNthSensorInfo(int) const;
    HitSensorInfo* getSensorInfo(const char *) const;

    void registHitSensorInfo(HitSensorInfo *);

    s32 mSensorInfosSize; // _0
    s32 mSensorCount; // _4
    HitSensorInfo** mSensorInfos; // _8
    u32 _C;
    u32 _10;
};
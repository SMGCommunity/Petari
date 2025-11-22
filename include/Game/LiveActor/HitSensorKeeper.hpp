#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class HitSensor;
class HitSensorInfo;
class LiveActor;

class HitSensorKeeper {
public:
    HitSensorKeeper(int);

    HitSensor* add(const char*, u32, u16, f32, LiveActor*, const TVec3f&);
    HitSensor* addPos(const char*, u32, u16, f32, LiveActor*, const TVec3f*, const TVec3f&);
    HitSensor* addMtx(const char*, u32, u16, f32, LiveActor*, Mtx, const TVec3f&);
    HitSensor* addCallback(const char*, u32, u16, f32, LiveActor*);

    HitSensor* getSensor(const char*) const;
    void update();
    void doObjCol();
    void clear();
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    HitSensorInfo* getNthSensorInfo(int) const;
    HitSensorInfo* getSensorInfo(const char*) const;

    void registHitSensorInfo(HitSensorInfo*);

    s32 mSensorInfosSize;          // 0x0
    s32 mSensorCount;              // 0x4
    HitSensorInfo** mSensorInfos;  // 0x8
    HitSensor* mTaking;            // 0xc
    HitSensor* mTaken;             // 0x10
};

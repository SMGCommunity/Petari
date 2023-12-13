#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"
#include <revolution.h>

class LiveActor;
class SensorGroup;

class HitSensor {
public:
    inline HitSensor() {}
    HitSensor(u32, u32, f32, LiveActor *);

    bool receiveMessage(u32, HitSensor *);
    void setType(u32);
    bool isType(u32) const;
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void addHitSensor(HitSensor *);

    u32 mSensorType; // _0
    TVec3f mPosition; // _4
    f32 mRadius; // _10
    u16 mSensorCount; // _14
    u16 mGroupSize; // _16
    HitSensor** mSensors; // _18
    SensorGroup* mSensorGroup; // _1C
    bool mValidBySystem; // _20
    bool mValidByHost; // _21
    u8 _22; // Padding?
    u8 _23; // Padding?
    LiveActor* mActor; // _24
};
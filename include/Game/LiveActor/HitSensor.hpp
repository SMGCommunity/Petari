#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"
#include <revolution.h>

class LiveActor;
class SensorGroup;

class HitSensor {
public:
    inline HitSensor() {}
    HitSensor(u32, u16, f32, LiveActor *);

    bool receiveMessage(u32, HitSensor *);
    void setType(u32);
    bool isType(u32) const;
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void addHitSensor(HitSensor *);

    u32 mType; // 0x0
    TVec3f mPosition; // 0x4
    f32 mRadius; // 0x10
    u16 mSensorCount; // 0x14
    u16 mGroupSize; // 0x16
    HitSensor** mSensors; // 0x18
    SensorGroup* mSensorGroup; // 0x1C
    bool mValidBySystem; // 0x20
    bool mValidByHost; // 0x21
    u8 _22; // Padding?
    u8 _23; // Padding?
    LiveActor* mActor; // 0x24
};
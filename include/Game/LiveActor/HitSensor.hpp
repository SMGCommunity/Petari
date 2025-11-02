#pragma once

#include <JSystem/JGeometry/TVec.hpp>
#include <revolution/types.h>

class LiveActor;
class SensorGroup;

class HitSensor {
public:
    HitSensor(u32 type, u16 groupSize, f32 radius, LiveActor* pHost);

    bool receiveMessage(u32 msg, HitSensor* pSender);
    void setType(u32);
    bool isType(u32) const;
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void addHitSensor(HitSensor* pSensor);

    /* 0x00 */ u32 mType;
    /* 0x04 */ TVec3f mPosition;
    /* 0x10 */ f32 mRadius;
    /* 0x14 */ u16 mSensorCount;
    /* 0x16 */ u16 mGroupSize;
    /* 0x18 */ HitSensor** mSensors;
    /* 0x1C */ SensorGroup* mSensorGroup;
    /* 0x20 */ bool mValidBySystem;
    /* 0x21 */ bool mValidByHost;
    /* 0x24 */ LiveActor* mHost;
};

#ifndef HITSENSOR_H
#define HITSENSOR_H

#include <revolution.h>

class LiveActor;
class SensorGroup;

enum SensorType
{
    SensorType_C = 0xC,
    SensorType_D = 0xD,
    SensorType_Fast = 0xF,
    SensorType_Slow = 0x10,
    SensorType_11 = 0x11,
    SensorType_1D = 0x1D,
    SensorType_MapObj = 0x46,
    SensorType_MoveCollision = 0x48,
    SensorType_ClipPartsObj = 0x49,
    SensorType_57 = 0x57,
    SensorType_58 = 0x58,
    SensorType_Press = 0x76,
    SensorType_7F = 0x7F
};

class HitSensor
{
public:
    inline HitSensor() { }
    HitSensor(u32, u16, f32, LiveActor *);

    u32 recieveMessage(u32, HitSensor *);
    void setType(u32);
    bool isType(u32) const;
    void validate();
    void invalidate();
    void validateBySystem();
    void invalidateBySystem();
    void addHitSensor(HitSensor *);

    u32 mSensorType; // _0
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    u16 mNumSensors; // _14
    u16 _16;
    HitSensor** mSensors; // _18
    SensorGroup* mSensorGroup; // _1C
    u8 _20;
    u8 _21;
    u8 _22;
    u8 _23;
    LiveActor* mParentActor; // _24
};

#endif // HITSENSOR_H
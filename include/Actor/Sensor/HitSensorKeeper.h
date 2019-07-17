#ifndef HITSENSORKEEPER_H
#define HITSENSORKEEPER_H

#include "Actor/Sensor/HitSensor.h"

class HitSensorKeeper
{
public:
    HitSensorKeeper(s32);

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

#endif // HITSENSORKEEPER_H
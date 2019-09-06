#ifndef SENSORGROUP_H
#define SENSORGROUP_H

#include "types.h"
#include "Actor/Sensor/HitSensor.h"

class SensorGroup
{
public:
    SensorGroup(s32, const char *);

    void add(HitSensor *);
    void remove(HitSensor *);
    void clear() const;

    s32 _0;
    s32 mNumSensors; // _4
    HitSensor* mSensors; // _8

};

#endif // SENSORGROUP_H
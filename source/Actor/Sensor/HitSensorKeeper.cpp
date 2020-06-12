#include "Actor/Sensor/HitSensorKeeper.h"

HitSensorKeeper::HitSensorKeeper(s32 sensorCount)
{
    mNumSensors = sensorCount;
    _0 = 0;
    mSensors = 0;
    _C = 0;
    _10 = 0;

    mSensors = (HitSensor**)new HitSensor[sensorCount];
}
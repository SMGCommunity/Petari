#pragma once

#include "Game/LiveActor/HitSensor.h"
#include <revolution.h>

class HitSensor;

class SensorGroup {
public:
    SensorGroup(int, const char *);

    void add(HitSensor *);
    void remove(HitSensor *);
    void clear() const;

    s32 mMaxSensors; // _0
    s32 mSensorCount; // _4
    HitSensor** mSensors; // _8
};

namespace MR {
    void initHitSensorGroup(HitSensor *);
};
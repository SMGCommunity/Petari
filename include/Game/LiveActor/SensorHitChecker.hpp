#pragma once

#include "Game/NameObj/NameObj.h"
#include "Game/LiveActor/HitSensor.h"
#include <revolution.h>

class SensorGroup;
class HitSensor;

class SensorHitChecker : NameObj {
public:
    SensorHitChecker(const char *);

    virtual ~SensorHitChecker();

    virtual void init(const JMapInfoIter &);
    virtual void movement();

    void initGroup(HitSensor *);
    void doObjColGroup(SensorGroup *, SensorGroup *) const;
    void doObjColInSameGroup(SensorGroup *) const;

    void checkAttack(HitSensor *, HitSensor *) const;

    SensorGroup* mPlayerGroup;    // _C
    SensorGroup* mRideGroup;      // _10
    SensorGroup* mEyeGroup;       // _14
    SensorGroup* mSimpleGroup;    // _18
    SensorGroup* mMapObjGroup;    // _1C
    SensorGroup* mCharacterGroup; // _30
};

class SensorGroup {
public:
    SensorGroup(int, const char *);

    void add(HitSensor *);
    void remove(HitSensor *);
    void clear() const;

    s32 mMaxSensors;      // _0
    s32 mSensorCount;     // _4
    HitSensor** mSensors; // _8
};

namespace MR {
    void initHitSensorGroup(HitSensor *);
};
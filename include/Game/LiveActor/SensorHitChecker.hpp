#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/NameObj/NameObj.hpp"
#include <revolution.h>

class SensorGroup;
class HitSensor;

class SensorHitChecker : public NameObj {
public:
    SensorHitChecker(const char*);

    virtual ~SensorHitChecker();

    virtual void init(const JMapInfoIter&) override;
    virtual void movement() override;

    void initGroup(HitSensor*);
    void doObjColGroup(SensorGroup*, SensorGroup*) const;
    void doObjColInSameGroup(SensorGroup*) const;

    void checkAttack(HitSensor*, HitSensor*) const;

    SensorGroup* mPlayerGroup;     // 0xC
    SensorGroup* mRideGroup;       // 0x10
    SensorGroup* mEyeGroup;        // 0x14
    SensorGroup* mSimpleGroup;     // 0x18
    SensorGroup* mMapObjGroup;     // 0x1C
    SensorGroup* mCharacterGroup;  // 0x30
};

class SensorGroup {
public:
    SensorGroup(int, const char*);

    void add(HitSensor*);
    void remove(HitSensor*);
    void clear() const;

    s32 mMaxSensors;       // 0x0
    s32 mSensorCount;      // 0x4
    HitSensor** mSensors;  // 0x8
};

namespace MR {
    void initHitSensorGroup(HitSensor*);
};

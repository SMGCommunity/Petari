#pragma once

#include "Game/NameObj/NameObj.hpp"

class SensorGroup;
class HitSensor;

class SensorHitChecker : public NameObj {
public:
    SensorHitChecker(const char*);

    /* 0x08 */ virtual ~SensorHitChecker();

    /* 0x0C */ virtual void init(const JMapInfoIter&) override;
    /* 0x14 */ virtual void movement() override;

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
};  // namespace MR

#pragma once

#include "Game/NameObj/NameObj.hpp"

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

    /* 0x0C */ SensorGroup* mPlayerGroup;
    /* 0x10 */ SensorGroup* mRideGroup;
    /* 0x14 */ SensorGroup* mEyeGroup;
    /* 0x18 */ SensorGroup* mSimpleGroup;
    /* 0x1C */ SensorGroup* mMapObjGroup;
    /* 0x20 */ SensorGroup* mCharacterGroup;
};

class SensorGroup {
public:
    SensorGroup(int, const char*);

    void add(HitSensor*);
    void remove(HitSensor*);
    void clear() const;

    /* 0x00 */ s32 mMaxSensors;
    /* 0x04 */ s32 mSensorCount;
    /* 0x08 */ HitSensor** mSensors;
};

namespace MR {
    void initHitSensorGroup(HitSensor*);
};  // namespace MR

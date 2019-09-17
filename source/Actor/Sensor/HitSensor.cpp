#include "Actor/Sensor/HitSensor.h"
#include "Actor/Sensor/SensorGroup.h"
#include "Actor/LiveActor/LiveActor.h"
#include "MR/SensorUtil.h"
#include "defines.h"

HitSensor::HitSensor(u32 type, u16 a2, f32 a3, LiveActor *pActor)
{
    // todo -- missing clrlwi on second arg
    mSensorType = type;
    _4 = 0.0f;
    _8 = 0.0f;
    _C = 0.0f;
    _10 = a3;
    mNumSensors = 0;
    _16 = a2;
    mSensors = 0;
    mSensorGroup = 0;
    _20 = 0;
    _21 = 1;
    mParentActor = pActor;

    if (a2 != 0)
    {
        mSensors = (HitSensor**)new HitSensor[a2];
        s32 curSensor = 0;

        while(curSensor < _16)
        {
            mSensors[curSensor] = 0;
            curSensor++;
        }
    }

    MR::initHitSensorGroup(this);
}

u32 HitSensor::recieveMessage(u32 msg, HitSensor *pOther)
{
    return mParentActor->receiveMessage(msg, pOther, this);
}

void HitSensor::setType(u32 type)
{
    bool wasRemoved = 0;

    u8 val = _20;
    mSensorType = type;

    if (val != 0)
    {
        if (_16 != 0)
        {
            if (_21 != 0)
            {
                mSensorGroup->remove(this);
                wasRemoved = true;
            }
        }
    }

    MR::initHitSensorGroup(this);

    if (wasRemoved != 0)
    {
        mSensorGroup->add(this);
    }

    mNumSensors = 0;
}

u32 HitSensor::isType(u32 type) const
{
    return __cntlzw(mSensorType - type) >> 5;
}

void HitSensor::validate()
{
    if (_21)
    {
        return;
    }

    _21 = 1;

    if (!_16)
    {
        return;
    }

    if (!_20)
    {
        return;
    }

    mSensorGroup->add(this);
}

void HitSensor::invalidate()
{
    if (_21)
    {
        _21 = 0;

        if (_16)
        {
            if (_20)
            {
                mSensorGroup->remove(this);
            }
        }
    }

    mNumSensors = 0;
}

void HitSensor::validateBySystem()
{
    if (!_20)
    {
        if (_16)
        {
            if (_21)
            {
                mSensorGroup->add(this);
            }
        }

        _20 = 1;
    }
}

void HitSensor::invalidateBySystem()
{
    if (!_20)
    {
        if (_16)
        {
            if (_21)
            {
                mSensorGroup->remove(this);
            }
        }

        _20 = 0;
        mNumSensors = 0;
    }
}

void HitSensor::addHitSensor(HitSensor *sensor)
{
    u16 val = mNumSensors;
    u16 val2 = _16;

    if (val >= val2)
    {
        return;
    }

    mSensors[val] = sensor;
    mNumSensors++;
}
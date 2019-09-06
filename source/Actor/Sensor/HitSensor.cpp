#include "Actor/Sensor/HitSensor.h"
#include "Actor/Sensor/SensorGroup.h"
#include "Actor/LiveActor/LiveActor.h"
#include "MR/SensorUtil.h"
#include "defines.h"

HitSensor::HitSensor(u32 type, u16 a2, f32 a3, LiveActor *actor)
{
    // todo -- missing clrlwi on second arg
    this->mSensorType = type;
    this->_4 = 0.0f;
    this->_8 = 0.0f;
    this->_C = 0.0f;
    this->_10 = a3;
    this->mNumSensors = 0;
    this->_16 = a2;
    this->mSensors = 0;
    this->mSensorGroup = 0;
    this->_20 = 0;
    this->_21 = 1;
    this->mParentActor = actor;

    if (a2 != 0)
    {
        this->mSensors = (HitSensor**)new HitSensor[a2];
        s32 curSensor = 0;

        while(curSensor < this->_16)
        {
            this->mSensors[curSensor] = 0;
            curSensor++;
        }
    }

    MR::initHitSensorGroup(this);
}

u32 HitSensor::recieveMessage(u32 msg, HitSensor *other)
{
    return this->mParentActor->receiveMessage(msg, other, this);
}

void HitSensor::setType(u32 type)
{
    bool wasRemoved = 0;

    u8 val = this->_20;
    this->mSensorType = type;

    if (val != 0)
    {
        if (this->_16 != 0)
        {
            if (this->_21 != 0)
            {
                this->mSensorGroup->remove(this);
                wasRemoved = true;
            }
        }
    }

    MR::initHitSensorGroup(this);

    if (wasRemoved != 0)
    {
        this->mSensorGroup->add(this);
    }

    this->mNumSensors = 0;
}

u32 HitSensor::isType(u32 type) const
{
    return __cntlzw(this->mSensorType - type) >> 5;
}

void HitSensor::validate()
{
    if (this->_21)
        return;

    this->_21 = 1;

    if (!this->_16)
        return;

    if (!this->_20)
        return;

    this->mSensorGroup->add(this);
}

void HitSensor::invalidate()
{
    if (this->_21)
    {
        this->_21 = 0;

        if (this->_16)
        {
            if (this->_20)
            {
                this->mSensorGroup->remove(this);
            }
        }
    }

    this->mNumSensors = 0;
}

void HitSensor::validateBySystem()
{
    if (!this->_20)
    {
        if (this->_16)
        {
            if (this->_21)
            {
                this->mSensorGroup->add(this);
            }
        }

        this->_20 = 1;
    }
}

void HitSensor::invalidateBySystem()
{
    if (!this->_20)
    {
        if (this->_16)
        {
            if (this->_21)
            {
                this->mSensorGroup->remove(this);
            }
        }

        this->_20 = 0;
        this->mNumSensors = 0;
    }
}

void HitSensor::addHitSensor(HitSensor *sensor)
{
    u16 val = this->mNumSensors;
    u16 val2 = this->_16;

    if (val >= val2)
        return;

    this->mSensors[val] = sensor;
    this->mNumSensors++;
}
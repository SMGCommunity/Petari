#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/SensorHitChecker.hpp"

HitSensor::HitSensor(u32 type, u16 groupSize, f32 radius, LiveActor* pHost) :
    mType(type),
    mPosition(0.0f, 0.0f, 0.0f),
    mRadius(radius),
    mSensorCount(0),
    mGroupSize(groupSize),
    mSensors(nullptr),
    mSensorGroup(nullptr),
    mValidBySystem(false),
    mValidByHost(true),
    mHost(pHost)
{
    if (mGroupSize != 0) {
        mSensors = new HitSensor*[mGroupSize];

        for (s32 i = 0; i < mGroupSize; i++) {
            mSensors[i] = nullptr;
        }
    }

    MR::initHitSensorGroup(this);
}

bool HitSensor::receiveMessage(u32 msg, HitSensor* pSender) {
    return mHost->receiveMessage(msg, pSender, this);
}

void HitSensor::setType(u32 type) {
    bool wasRemoved = false;

    mType = type;

    if (mValidBySystem) {
        if (mGroupSize != 0 && mValidByHost) {
            mSensorGroup->remove(this);
            wasRemoved = true;
        }
    }

    MR::initHitSensorGroup(this);

    if (wasRemoved) {
        mSensorGroup->add(this);
    }

    mSensorCount = 0;
}

bool HitSensor::isType(u32 type) const {
    return mType == type;
}

void HitSensor::validate() {
    if (!mValidByHost) {
        mValidByHost = true;

        if (mGroupSize != 0 && mValidBySystem) {
            mSensorGroup->add(this);
        }
    }
}

void HitSensor::invalidate() {
    if (mValidByHost) {
        mValidByHost = false;

        if (mGroupSize != 0 && mValidBySystem) {
            mSensorGroup->remove(this);
        }
    }

    mSensorCount = 0;
}

void HitSensor::validateBySystem() {
    if (!mValidBySystem) {
        if (mGroupSize != 0 && mValidByHost) {
            mSensorGroup->add(this);
        }

        mValidBySystem = true;
    }
}

void HitSensor::invalidateBySystem() {
    if (mValidBySystem) {
        if (mGroupSize != 0 && mValidByHost) {
            mSensorGroup->remove(this);
        }

        mValidBySystem = false;
        mSensorCount = 0;
    }
}

void HitSensor::addHitSensor(HitSensor* pSensor) {
    if (mSensorCount < mGroupSize) {
        mSensors[mSensorCount] = pSensor;
        mSensorCount++;
    }
}

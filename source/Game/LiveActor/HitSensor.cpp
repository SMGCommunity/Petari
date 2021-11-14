#include "Game/LiveActor/HitSensor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/SensorHitChecker.h"

// Doesn't Match
HitSensor::HitSensor(u32 type, u16 maxSensors, f32 radius, LiveActor *pActor) {
    mSensorType = type;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mRadius = radius;
    mSensorCount = 0;
    mMaxSensors = maxSensors;
    mSensors = 0;
    mSensorGroup = NULL;
    mValidBySystem = false;
    mValidByHost = true;
    mActor = pActor;

    if (maxSensors) {
        mSensors = new HitSensor*[maxSensors];

        for (s32 i = 0; i < mMaxSensors; i++) {
            mSensors[i] = NULL;
        }
    }

    MR::initHitSensorGroup(this);
}

u32 HitSensor::receiveMessage(u32 msg, HitSensor *pReceiver) {
    return mActor->receiveMessage(msg, pReceiver, this);
}

void HitSensor::setType(u32 type) {
    bool wasRemoved = false;

    mSensorType = type;

    if (mValidBySystem) {
        if (mMaxSensors) {
            if (mValidByHost){
                mSensorGroup->remove(this);
                wasRemoved = true;
            }
        }
    }

    MR::initHitSensorGroup(this);

    if (wasRemoved) {
        mSensorGroup->add(this);
    }

    mSensorCount = 0;
}

bool HitSensor::isType(u32 type) const {
    return !(type - mSensorType);
}


void HitSensor::validate() {
    if (mValidByHost) {
        return;
    }

    mValidByHost = true;

    if (!mMaxSensors) {
        return;
    }

    if (!mValidBySystem) {
        return;
    }

    mSensorGroup->add(this);
}

void HitSensor::invalidate() {
    if (mValidByHost) {
        mValidByHost = false;

        if (mMaxSensors) {
            if (mValidBySystem) {
                mSensorGroup->remove(this);
            }
        }
    }

    mSensorCount = 0;
}

void HitSensor::validateBySystem() {
    if (!mValidBySystem) {
        if (mMaxSensors) {
            if (mValidByHost) {
                mSensorGroup->add(this);
            }
        }

        mValidBySystem = true;
    }
}

void HitSensor::invalidateBySystem() {
    if (!mValidBySystem) {
        if (mMaxSensors) {
            if (mValidByHost) {
                mSensorGroup->remove(this);
            }
        }

        mValidBySystem = false;
        mSensorCount = 0;
    }
}

void HitSensor::addHitSensor(HitSensor *pSensor) {
    if (mSensorCount < mMaxSensors) {
        mSensors[mSensorCount] = pSensor;
        mSensorCount++;
    }
}
#include "Game/LiveActor/HitSensor.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/SensorHitChecker.h"

// fuck this shit, sensorGroupSize is getting turned into a u32 becuase literally nothing else will match
// whatever
HitSensor::HitSensor(u32 type, u32 sensorGroupSize, f32 radius, LiveActor *pActor) {
    mSensorType = type;
    mPosition.x = 0.0f;
    mPosition.y = 0.0f;
    mPosition.z = 0.0f;
    mRadius = radius;
    mSensorCount = 0;
    mGroupSize = sensorGroupSize;
    mSensors = 0;
    mSensorGroup = nullptr;
    mValidBySystem = false;
    mValidByHost = true;
    mActor = pActor;

    if (sensorGroupSize & 0xFFFF) {
        mSensors = new HitSensor*[sensorGroupSize];

        for (s32 i = 0; i < mGroupSize; i++) {
            mSensors[i] = nullptr;
        }
    }

    MR::initHitSensorGroup(this);
}

bool HitSensor::receiveMessage(u32 msg, HitSensor *pReceiver) {
    return mActor->receiveMessage(msg, pReceiver, this);
}

void HitSensor::setType(u32 type) {
    bool wasRemoved = false;

    mSensorType = type;

    if (mValidBySystem) {
        if (mGroupSize) {
            if (mValidByHost) {
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

    if (!mGroupSize) {
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

        if (mGroupSize) {
            if (mValidBySystem) {
                mSensorGroup->remove(this);
            }
        }
    }

    mSensorCount = 0;
}

void HitSensor::validateBySystem() {
    if (!mValidBySystem) {
        if (mGroupSize) {
            if (mValidByHost) {
                mSensorGroup->add(this);
            }
        }

        mValidBySystem = true;
    }
}

void HitSensor::invalidateBySystem() {
    if (!mValidBySystem) {
        if (mGroupSize) {
            if (mValidByHost) {
                mSensorGroup->remove(this);
            }
        }

        mValidBySystem = false;
        mSensorCount = 0;
    }
}

void HitSensor::addHitSensor(HitSensor *pSensor) {
    if (mSensorCount < mGroupSize) {
        mSensors[mSensorCount] = pSensor;
        mSensorCount++;
    }
}
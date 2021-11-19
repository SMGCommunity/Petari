#include "Game/LiveActor/SensorHitChecker.h"

SensorGroup::SensorGroup(int maxSensors, const char *a2) {
    mMaxSensors = maxSensors;
    mSensorCount = 0;
    mSensors = NULL;
    mSensors = new HitSensor*[maxSensors];

    for (s32 i = 0; i < mMaxSensors; i++) {
        mSensors[i] = NULL;
    }
}

void SensorGroup::add(HitSensor *pSensor) {
    mSensors[mSensorCount] = pSensor;
    mSensorCount++;
    pSensor->mSensorGroup = this;
}

void SensorGroup::remove(HitSensor *pSensor) {
    for (s32 i = 0; i < mSensorCount; i++) {
        if (mSensors[i] == pSensor) {
            u32 count = mSensorCount -1;
            mSensors[i] = mSensors[count];
            mSensorCount--;
            break;
        }
    }
}

void SensorGroup::clear() const {
    for (s32 i = 0; i < mSensorCount; i++) {
        mSensors[i]->mSensorCount = NULL;
    }
}
#include "Game/LiveActor/HitSensorKeeper.h"
#include "Game/LiveActor/HitSensorInfo.h"

HitSensorKeeper::HitSensorKeeper(int sensorCount) {
    mSensorCount = sensorCount;
    _0 = 0;
    mSensors = NULL;
    _C = 0;
    _10 = 0;
    mSensors = new HitSensor*[sensorCount];

    for (s32 i = 0; i < mSensorCount; i++) {
        mSensors[i] = NULL;
    }
}

HitSensor* HitSensorKeeper::add(const char *pName, u32 sensorType, u16 maxSensors, f32 radius, LiveActor *pActor, const TVec3f &a6) {
    HitSensorInfo* pInfo = new HitSensorInfo(pName, new HitSensor(sensorType, maxSensors, radius, pActor), NULL, NULL, a6, false);
    registHitSensorInfo(pInfo);
    return pInfo->mSensor;
}
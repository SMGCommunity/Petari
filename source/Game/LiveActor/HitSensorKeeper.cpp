#include "Game/LiveActor/HitSensorKeeper.h"
#include "Game/LiveActor/HitSensorInfo.h"
#include "Game/Util.h"
#include <string.h>

HitSensorKeeper::HitSensorKeeper(int sensorCount) {
    mSensorCount = sensorCount;
    mSensorInfosSize = 0;
    mSensorInfos = NULL;
    _C = 0;
    _10 = 0;
    mSensorInfos = new HitSensorInfo*[sensorCount];

    for (s32 i = 0; i < mSensorCount; i++) {
        mSensorInfos[i] = NULL;
    }
}

HitSensor* HitSensorKeeper::add(const char *pName, u32 sensorType, u16 sensorGroupSize, f32 radius, LiveActor *pActor, const TVec3f &a6) {
    HitSensorInfo* pInfo = new HitSensorInfo(pName, new HitSensor(sensorType, sensorGroupSize, radius, pActor), NULL, NULL, a6, false);
    registHitSensorInfo(pInfo);
    return pInfo->mSensor;
}

HitSensor* HitSensorKeeper::addPos(const char *pName, u32 sensorType, u16 sensorGroupSize, f32 radius, LiveActor * pActor, const TVec3f *pPos, const TVec3f &a7) {
    HitSensorInfo* pInfo = new HitSensorInfo(pName, new HitSensor(sensorType, sensorGroupSize, radius, pActor), pPos, NULL, a7, false);
    registHitSensorInfo(pInfo);
    return pInfo->mSensor;
}

HitSensor* HitSensorKeeper::addMtx(const char *pName, u32 sensorType, u16 sensorGroupSize, f32 radius, LiveActor *pActor, MtxPtr mtx, const TVec3f &a7) {
    HitSensorInfo* pInfo = new HitSensorInfo(pName, new HitSensor(sensorType, sensorGroupSize, radius, pActor), NULL, mtx, a7, false);
    registHitSensorInfo(pInfo);
    return pInfo->mSensor;
}

HitSensor* HitSensorKeeper::addCallback(const char *pName, u32 sensorType, u16 sensorGroupSize, f32 radius, LiveActor *pActor) {
    HitSensorInfo* pInfo = new HitSensorInfo(pName, new HitSensor(sensorType, sensorGroupSize, radius, pActor), NULL, NULL, TVec3f(0.0f), true);
    registHitSensorInfo(pInfo);
    return pInfo->mSensor;
}

HitSensor* HitSensorKeeper::getSensor(const char *pSensorName) const {
    if (mSensorInfosSize == 1) {
        return mSensorInfos[0]->mSensor;
    }

    u32 hashCode = MR::getHashCode(pSensorName);

    for (s32 i = 0; i < mSensorInfosSize; i++) {
        if (hashCode == mSensorInfos[i]->mHashCode && !strstr(mSensorInfos[i]->mName, pSensorName)) {
            return mSensorInfos[i]->mSensor;
        }
    }

    return NULL;
}

void HitSensorKeeper::update() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->update();
    }
}

void HitSensorKeeper::doObjCol() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->doObjCol();
    }
}

void HitSensorKeeper::clear() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        HitSensorInfo* pInfo = mSensorInfos[i];
        pInfo->mSensor->mSensorCount = 0;
    }
}

void HitSensorKeeper::validate() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->mSensor->validate();
    }
}

void HitSensorKeeper::invalidate() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->mSensor->invalidate();
    }
}

void HitSensorKeeper::validateBySystem() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->mSensor->validateBySystem();
    }
}

void HitSensorKeeper::invalidateBySystem() {
    for (s32 i = 0; i < mSensorInfosSize; i++) {
        mSensorInfos[i]->mSensor->invalidateBySystem();
    }
}

HitSensorInfo* HitSensorKeeper::getNthSensorInfo(int n) const {
    return mSensorInfos[n];
}

HitSensorInfo* HitSensorKeeper::getSensorInfo(const char *pSensorName) const {
    if (mSensorInfosSize == 1) {
        return mSensorInfos[0];
    }

    u32 hashCode = MR::getHashCode(pSensorName);

    for (s32 i = 0; i < mSensorInfosSize; i++) {
        if (hashCode == mSensorInfos[i]->mHashCode && !strstr(mSensorInfos[i]->mName, pSensorName)) {
            return mSensorInfos[i];
        }
    }

    return NULL;
}

void HitSensorKeeper::registHitSensorInfo(HitSensorInfo *pInfo) {
    mSensorInfos[mSensorInfosSize] = pInfo;
    mSensorInfosSize++;
    pInfo->update();
}
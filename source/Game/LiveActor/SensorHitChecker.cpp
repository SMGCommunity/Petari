#include "Game/LiveActor/SensorHitChecker.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"

void SensorHitChecker::init(const JMapInfoIter &rIter) {
    MR::connectToScene(this, 5, -1, -1, -1);
}

void SensorHitChecker::initGroup(HitSensor *pSensor) {
    if (MR::isSensorPlayer(pSensor)) {
        pSensor->mSensorGroup = mPlayerGroup;
    }
    else if (MR::isSensorRide(pSensor)) {
        pSensor->mSensorGroup = mRideGroup;
    }
    else if (pSensor->isType(0x7F)) {
        pSensor->mSensorGroup = mRideGroup;
    }
    else if (pSensor->isType(0x4A) || pSensor->isType(0x4C) 
            || pSensor->isType(0x15) || pSensor->isType(0x47) 
            || pSensor->isType(0x1F) || MR::isSensorRush(pSensor) || MR::isSensorAutoRush(pSensor)) {
        pSensor->mSensorGroup = mSimpleGroup;
    }
    else {
        if (MR::isSensorMapObj(pSensor)) {
            pSensor->mSensorGroup = mMapObjGroup;
        }
        else {
            pSensor->mSensorGroup = mCharacterGroup;
        }
    }
}

void SensorHitChecker::movement() {
    mPlayerGroup->clear();
    mRideGroup->clear();
    mEyeGroup->clear();
    mSimpleGroup->clear();
    mMapObjGroup->clear();
    mCharacterGroup->clear();
    doObjColGroup(mPlayerGroup, mCharacterGroup);
    doObjColGroup(mPlayerGroup, mMapObjGroup);
    doObjColGroup(mPlayerGroup, mRideGroup);
    doObjColGroup(mPlayerGroup, mSimpleGroup);
    doObjColGroup(mPlayerGroup, mEyeGroup);
    doObjColGroup(mRideGroup, mCharacterGroup);
    doObjColGroup(mRideGroup, mMapObjGroup);
    doObjColGroup(mRideGroup, mSimpleGroup);
    doObjColGroup(mRideGroup, mEyeGroup);
    doObjColGroup(mEyeGroup, mCharacterGroup);
    doObjColGroup(mEyeGroup, mMapObjGroup);
    doObjColGroup(mEyeGroup, mSimpleGroup);
    doObjColGroup(mCharacterGroup, mMapObjGroup);
    doObjColInSameGroup(mCharacterGroup);
}

#ifdef NON_MATCHING // Wrong registers
void SensorHitChecker::doObjColGroup(SensorGroup *pGroup1, SensorGroup *pGroup2) const {
    s32 group1SensorCount = pGroup1->mSensorCount;
    for (s32 i = 0; i < group1SensorCount; i++) {
        HitSensor* curGroup1Sensor = pGroup1->mSensors[i];
        bool group1Sensorvalidated = false;

        if (curGroup1Sensor->mValidByHost && curGroup1Sensor->mValidBySystem) {
            group1Sensorvalidated = true;
        }

        if (group1Sensorvalidated && !MR::isClipped(curGroup1Sensor->mActor)) {
            s32 group2SensorCount = pGroup2->mSensorCount;
            for (s32 x = 0; x < group2SensorCount; x++) {
                HitSensor* curGroup2Sensor = pGroup2->mSensors[x];
                bool group2Validated = false;

                if (curGroup2Sensor->mValidByHost && curGroup2Sensor->mValidBySystem) {
                    group2Validated = true;
                }

                if (group2Validated && !MR::isClipped(curGroup2Sensor->mActor)) {
                    checkAttack(curGroup1Sensor, curGroup2Sensor);
                }
            }
        }
    }
}
#endif 

#ifdef NON_MATCHING // Same register issue
void SensorHitChecker::doObjColInSameGroup(SensorGroup *pSensorGroup) const {
    s32 sensorGroupCount = pSensorGroup->mSensorCount;
    for (s32 i = 0; i < sensorGroupCount; i++) {
        HitSensor* pFirstSensor = pSensorGroup->mSensors[i];
        bool isFirstSensorValid = false;

        if (pFirstSensor->mValidByHost && pFirstSensor->mValidBySystem) {
            isFirstSensorValid = true;
        }

        if (isFirstSensorValid && !MR::isClipped(pFirstSensor->mActor)) {
            for (s32 x = 0; x < sensorGroupCount; x++) {
                HitSensor* pSecondSensor = pSensorGroup->mSensors[x];
                bool isSecondSensorValid = false;

                if (pSecondSensor->mValidByHost && pSecondSensor->mValidBySystem) {
                    isSecondSensorValid = true;
                }

                if (isSecondSensorValid && !MR::isClipped(pSecondSensor->mActor)) {
                    checkAttack(pFirstSensor, pSecondSensor);
                }
            }
        }
    }
}
#endif

#ifdef NON_MATCHING // Wrong registers
void SensorHitChecker::checkAttack(HitSensor *pSensor1, HitSensor *pSensor2) const {
    if (pSensor1->mActor != pSensor2->mActor) {
        f32 xPos = pSensor1->mPosition.x - pSensor2->mPosition.x;
        f32 yPos = pSensor1->mPosition.y - pSensor2->mPosition.y;
        f32 zPos = pSensor1->mPosition.z - pSensor2->mPosition.z;
        f32 totalSize = pSensor2->mRadius + pSensor1->mRadius;

        if (!((((yPos * yPos) + (xPos * xPos)) + (zPos * zPos)) >= (totalSize * totalSize))) {
            if (!pSensor2->isType(127)) {
                pSensor1->addHitSensor(pSensor2);
            }

            if (!pSensor1->isType(127)) {
                pSensor2->addHitSensor(pSensor1);
            }
        }
    }
}
#endif 

namespace MR {
    void initHitSensorGroup(HitSensor *pSensor) {
        reinterpret_cast<SensorHitChecker*>(MR::getSceneObjHolder()->getObj(0))->initGroup(pSensor);
    }
};

SensorHitChecker::~SensorHitChecker() {}

SensorHitChecker::SensorHitChecker(const char *pName) : NameObj(pName) {
    mPlayerGroup = nullptr;
    mRideGroup = nullptr;
    mEyeGroup = nullptr;
    mSimpleGroup = nullptr;
    mMapObjGroup = nullptr;
    mCharacterGroup = nullptr;

    mPlayerGroup = new SensorGroup(0x10, "Player");
    mRideGroup = new SensorGroup(0x80, "Ride");
    mEyeGroup = new SensorGroup(0x200, "Eye");
    mSimpleGroup = new SensorGroup(0x800, "Simple");
    mMapObjGroup = new SensorGroup(0x400, "MapObj");
    mCharacterGroup = new SensorGroup(0x400, "Character");
}

SensorGroup::SensorGroup(int maxSensors, const char *a2) {
    mMaxSensors = maxSensors;
    mSensorCount = 0;
    mSensors = nullptr;
    mSensors = new HitSensor*[maxSensors];

    for (s32 i = 0; i < mMaxSensors; i++) {
        mSensors[i] = nullptr;
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
        mSensors[i]->mSensorCount = nullptr;
    }
}
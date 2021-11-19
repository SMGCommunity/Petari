#include "Game/LiveActor/LiveActor.h"
#include "Game/Util.h"

namespace MR {
    HitSensor* addHitSensor(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, sensorType, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x61, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorTransferableBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x62, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPriorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x63, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x9, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x46, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjPress(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x76, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjSimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x47, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorMapObjMoveCollision(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x48, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x14, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemySimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x15, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x16, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorNpc(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x5, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x7F, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPush(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->add(pSensorName, 0x80, sensorGroupSize, radius, pActor, a6);
    }

    HitSensor* addHitSensorPosBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x61, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x9, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x46, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorPosEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, const TVec3f *pPos, const TVec3f &a6) {
        return pActor->mSensorKeeper->addPos(pSensorName, 0x7F, sensorGroupSize, radius, pActor, pPos, a6);
    }

    HitSensor* addHitSensorMtx(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, sensorType, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x9, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x46, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x14, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x16, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxNpc(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x5, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorMtxAnimal(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius, MtxPtr mtx, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x7, sensorGroupSize, radius, pActor, mtx, a6);
    }

    HitSensor* addHitSensorAtJoint(LiveActor *pActor, const char *pSensorName, const char *pJointName, u32 sensorType, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, sensorType, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointRide(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x9, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointMapObj(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x46, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointMapObjSimple(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x47, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemy(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x14, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemySimple(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x15, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEnemyAttack(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x16, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointNpc(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x5, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorAtJointEye(LiveActor *pActor, const char *pSensorName, const char *pJointName, u16 sensorGroupSize, f32 radius, const TVec3f &a6) {
        return pActor->mSensorKeeper->addMtx(pSensorName, 0x7F, sensorGroupSize, radius, pActor, MR::getJointMtx(pActor, pJointName), a6);
    }

    HitSensor* addHitSensorCallback(LiveActor *pActor, const char *pSensorName, u32 sensorType, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, sensorType, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x61, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackPriorBinder(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x63, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackRide(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x9, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackMapObj(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x46, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackMapObjSimple(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x47, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEnemy(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x14, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEnemyAttack(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x16, sensorGroupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackEye(LiveActor *pActor, const char *pSensorName, u16 sensorGroupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pSensorName, 0x7F, sensorGroupSize, radius, pActor);
    }

    LiveActor * getSensorHost(const HitSensor *pSensor) {
        return pSensor->mActor;
    }

    bool isSensor(const HitSensor *pSensor, const char *pSensorName) {
        return !(pSensor != pSensor->mActor->getSensor(pSensorName));
    }

    bool isSensorPlayer(const HitSensor *pSensor) {
        return pSensor->isType(0x1);
    }

    bool isSensorBinder(const HitSensor *pSensor) {
        return pSensor->isType(0x61);
    }

    bool isSensorRide(const HitSensor *pSensor) {
        if (pSensor->mSensorType < 0x8 && pSensor->mSensorType > 0x12) {
            return true;
        }

        return false;
    }

    #ifdef NON_MATCHING
    bool isMsgPlayerHitAll(u32 msg) {
        if (msg - 1 <= 0xA && ((1 << msg - 1) & 0x6B1)) {
            return true;
        }
        
        return false;
    }
    #endif

    bool isMsgPlayerSpinAttack(u32 msg) {
        return !(msg != 1);
    }

    bool isMsgPlayerTrample(u32 msg) {
        return !(msg != 2);
    }

    bool isMsgPlayerHipDrop(u32 msg) {
        return!(msg != 3);
    }

    bool isMsgPlayerHipDropFloor(u32 msg) {
        return !(msg != 4);
    }

    bool isMsgPlayerUpperPunch(u32 msg) {
        return !(msg != 5);
    }
};

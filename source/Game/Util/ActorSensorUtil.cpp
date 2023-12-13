#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath/JMath.hpp"

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

    bool isSensorMapObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType < 0x45 && pSensor->mSensorType > 0x5F) {
            return true;
        }

        return false;
    }

    bool isSensorAutoRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType < 0x60 && pSensor->mSensorType > 0x6E) {
            return true;
        }

        return false;
    }

    bool isSensorRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType < 0x6F && pSensor->mSensorType > 0x74) {
            return true;
        }

        return false;
    }

    bool isSensorPressObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType < 0x75 && pSensor->mSensorType > 0x77) {
            return true;
        }

        return false;
    }

    bool isSensorEye(const HitSensor *pSensor) {
        return pSensor->isType(0x7F);
    }

    bool isSensorPush(const HitSensor *pSensor) {
        return pSensor->isType(0x80);
    }

    bool isSensorItem(const HitSensor *pSensor) {
        if (pSensor->isType(0x4A) || pSensor->isType(0x4B) || pSensor->isType(0x4D)) {
            return true;
        }

        return false;
    }

    bool tryGetItem(HitSensor *pSender, HitSensor *pReceiver) {
        return pReceiver->receiveMessage(0x87, pSender);
    }

    TVec3f* getSensorPos(const HitSensor *pSensor) {
        return (TVec3f*)&pSensor->mPosition;
    }

    void validateHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper) {
            pActor->mSensorKeeper->validate();
        }
    }

    void invalidateHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper) {
            pActor->mSensorKeeper->invalidate();
        }
    }

    bool sendArbitraryMsg(u32 msg, HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(msg, pSender);
    }

    bool sendMsgPush(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x29, pSender);
    }

    bool sendMsgPlayerTrample(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(2, pSender);
    }

    bool sendMsgPlayerPunch(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(1, pSender);
    }

    bool sendMsgJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2C, pSender);
    }

    bool sendMsgTouchJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2D, pSender);
    }

    bool sendMsgTaken(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x1F, pSender);
    }

    bool sendMsgKick(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2B, pSender);
    }

    bool sendMsgAwayJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x2F, pSender);
    }

    bool sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        TVec3f curSenderPos(pSender->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&pReceiver->mPosition, (Vec*)&rDir, (Vec*)&pSender->mPosition);
        bool ret = pReceiver->receiveMessage(msg, pSender);
        pSender->mPosition.set<f32>(curSenderPos);
        return ret;
    }

    bool sendMsgEnemyAttackFlipWeak(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4D, pSender);
    }

    bool sendMsgEnemyAttackFlipWeakJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4E, pSender);
    }

    bool sendMsgEnemyAttackFlip(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x50, pSender);
    }

    bool sendMsgEnemyAttackFlipToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x50, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFlipJump(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x4F, pSender);
    }

    bool sendMsgEnemyAttackFlipRot(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x51, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximum(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x52, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x52, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttack(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x53, pSender);
    }

    bool sendMsgEnemyAttackStrong(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x54, pSender);
    }

    bool sendMsgEnemyAttackStrongToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x54, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFire(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x58, pSender);
    }

    bool sendMsgEnemyAttackFireStrong(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x59, pSender);
    }

    bool sendMsgEnemyAttackElectric(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5A, pSender);
    }

    bool sendMsgEnemyAttackFreeze(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5D, pSender);
    }

    bool sendMsgEnemyAttackHeatBeam(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x5B, pSender);
    }

    bool sendMsgEnemyAttackExplosion(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x56, pSender);
    }

    bool sendMsgEnemyAttackCounterSpin(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x60, pSender);
    }

    bool sendMsgEnemyAttackCounterHipDrop(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x61, pSender);
    }

    bool sendMsgLockOnStarPieceShoot(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0xE, pSender);
    }

    bool sendMsgStarPieceAttack(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0xC, pSender);
    }

    bool sendMsgStarPieceGift(HitSensor *pReceiver, HitSensor *pSender, u32 msg) {
        return pReceiver->receiveMessage(msg + 0xE, pSender);
    }

    bool sendMsgEnemyAttackMaximum(HitSensor *pReceiver, HitSensor *pSender) {
        return pReceiver->receiveMessage(0x55, pSender);
    }

    bool sendMsgEnemyAttackMaximumToDir(HitSensor *pReceiver, HitSensor *pSender, const TVec3f &rDir) {
        return sendMsgEnemyAttackMsgToDir(0x55, pReceiver, pSender, rDir);
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

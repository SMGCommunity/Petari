#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Util.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"

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

    bool tryUpdateHitSensorsAll(LiveActor *pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->update();
            return true;
        }
        return false;
        
    }

    void updateHitSensorsAll(LiveActor *pActor) {
        pActor->mSensorKeeper->update();
    }

    HitSensor* getSensor(LiveActor *pActor, int n) {
        return pActor->mSensorKeeper->getNthSensorInfo(n)->mSensor;
    }

    HitSensor* getSensorWithIndex(LiveActor *pActor, int index) {
        return (pActor->mSensorKeeper->getNthSensorInfo(index))->mSensor;
    }

    HitSensor* getTaking(const LiveActor *pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            return pActor->mSensorKeeper->mTaking;
        }
        return 0;
    }

    HitSensor* getTaken(const LiveActor *pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            return pActor->mSensorKeeper->mTaken;
        }
        return 0;
    }

    void setSensorPos(HitSensor *pSensor, const TVec3f &rPos) {
        pSensor->mPosition.set(rPos);
    }

    // Minor mismatch: Wrong float registers used
    /* void setSensorOffset(LiveActor *pActor, const char *pName, const TVec3f &rOffset) {
        pActor->mSensorKeeper->getSensorInfo(pName)->_C.setPS2(rOffset);
    } */

    void setSensorRadius(LiveActor *pActor, const char *pName, float radius) {
        pActor->mSensorKeeper->getSensorInfo(pName)->mSensor->mRadius = radius;
    }

    void setHitSensorApart(HitSensor *pSensor1, HitSensor *pSensor2) {
        if (getTaking(pSensor1->mActor) == pSensor2 || getTaken(pSensor2->mActor) == pSensor1) {
            pSensor1->mActor->mSensorKeeper->mTaking = nullptr;
            pSensor2->mActor->mSensorKeeper->mTaken = nullptr;
        }
        else {
            pSensor1->mActor->mSensorKeeper->mTaken = nullptr;
            pSensor2->mActor->mSensorKeeper->mTaking = nullptr;
        }
    }

    void calcSensorDirection(TVec3f *pResult, const HitSensor *pSensor1, const HitSensor *pSensor2) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pSensor1->mPosition, (Vec*)&stack_8);
        pResult->set(stack_8);
    }

    void calcSensorDirectionNormalize(TVec3f *pResult, const HitSensor *pSensor1, const HitSensor *pSensor2) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pSensor1->mPosition, (Vec*)&stack_8);
        pResult->set(stack_8);
        normalizeOrZero(pResult);
    }

    void calcSensorHorizonNormalize(TVec3f *pResult, const TVec3f &a2, const HitSensor *pSensor1, const HitSensor *pSensor2) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pSensor1->mPosition, (Vec*)&stack_8);
        pResult->rejection(stack_8, a2);
        normalizeOrZero(pResult);
    }



    LiveActor* getSensorHost(const HitSensor *pSensor) {
        return pSensor->mActor;
    }

    bool isValidHitSensor(LiveActor *pActor, const char *pName) {
        return pActor->mSensorKeeper->getSensor(pName)->mValidByHost;
    }

    bool isSensor(const HitSensor *pSensor, const char *pSensorName) {
        return !(pSensor != pSensor->mActor->getSensor(pSensorName));
    }

    bool isSensorType(const HitSensor *pSensor, u32 type) {
        return pSensor->isType(type);
    }

    bool isSensorPlayer(const HitSensor *pSensor) {
        return pSensor->isType(0x1);
    }

    bool isSensorBinder(const HitSensor *pSensor) {
        return pSensor->isType(0x61);
    }

    bool isSensorRide(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x8 && pSensor->mSensorType < 0x12) {
            return true;
        }

        return false;
    }

    bool isSensorEnemy(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x13 && pSensor->mSensorType < 0x44) {
            return true;
        }
        return false;
    }

    bool isSensorEnemyAttack(const HitSensor *pSensor) {
        return pSensor->isType(0x16);
    }

    bool isSensorNpc(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x4 && pSensor->mSensorType < 0x6) {
            return true;
        }
        return false;
    }

    bool isSensorMapObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x45 && pSensor->mSensorType < 0x5F) {
            return true;
        }

        return false;
    }

    bool isSensorAutoRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x60 && pSensor->mSensorType < 0x6E) {
            return true;
        }

        return false;
    }

    bool isSensorRush(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x6F && pSensor->mSensorType < 0x74) {
            return true;
        }

        return false;
    }

    bool isSensorPressObj(const HitSensor *pSensor) {
        if (pSensor->mSensorType > 0x75 && pSensor->mSensorType < 0x77) {
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

    void validateHitSensor(LiveActor *pActor, const char *pName) {
        pActor->mSensorKeeper->getSensor(pName)->validate();
    }

    void invalidateHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper) {
            pActor->mSensorKeeper->invalidate();
        }
    }

    void invalidateHitSensor(LiveActor *pActor, const char *pName) {
        pActor->mSensorKeeper->getSensor(pName)->invalidate();
    }

    void clearHitSensors(LiveActor *pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->clear();
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

    bool sendMsgEnemyAttackToBindedSensor(LiveActor *pActor, HitSensor *pSensor) {
        return sendMsgToBindedSensor(0x53, pActor, pSensor);
    }

    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor *pActor, HitSensor *pSensor) {
        return sendMsgToBindedSensor(0x56, pActor, pSensor);
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

    bool sendMsgStartDemo(LiveActor *pActor) {
        return sendSimpleMsgToActor(0x6f, pActor);
    }

    bool sendSimpleMsgToActor(u32 msg, LiveActor *pActor) {
        return pActor->receiveMessage(msg, getMessageSensor(), getMessageSensor());
    }

    bool sendMsgToBindedSensor(u32 msg, HitSensor *pSensor) {
        return sendMsgToBindedSensor(msg, pSensor->mActor, pSensor);
    }

    bool sendMsgToGroundSensor(u32 msg, HitSensor *pSensor) {
        if (isBindedGround(pSensor->mActor)) {
            return getGroundSensor(pSensor->mActor)->receiveMessage(msg, pSensor);
        }
        return false;
    }

    bool sendMsgToWallSensor(u32 msg, HitSensor *pSensor) {
        if (isBindedWall(pSensor->mActor)) {
            return getWallSensor(pSensor->mActor)->receiveMessage(msg, pSensor);
        }
        return false;
    }

    bool sendMsgToEnemyAttackBlow(HitSensor *pSensor1, HitSensor *pSensor2) {
        return pSensor1->receiveMessage(0x62, pSensor2);
    }

    bool sendMsgToEnemyAttackTrample(HitSensor *pSensor1, HitSensor *pSensor2) {
        return pSensor1->receiveMessage(0x63, pSensor2);
    }

    bool sendMsgToEnemyAttackBlowOrTrample(HitSensor *pSensor1, HitSensor *pSensor2, f32 a3) {
        TVec3f stack_8(pSensor1->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pSensor2->mPosition, (Vec*)&stack_8);
        normalizeOrZero(&stack_8);
        if (a3 < pSensor2->mActor->mGravity.dot(stack_8)) {
            return pSensor1->receiveMessage(0x63, pSensor2);
        }
        return pSensor1->receiveMessage(0x62, pSensor2);
    }

    bool sendMsgToEnemyAttackShockWave(HitSensor *pSensor1, HitSensor *pSensor2) {
        return pSensor1->receiveMessage(0x64, pSensor2);
    }

    void sendMsgToGroupMember(u32 msg, LiveActor *pActor, HitSensor *pSensor, const char *pName) {
        LiveActorGroup* group = getGroupFromArray(pActor);
        if (group != nullptr) {
            static_cast<MsgSharedGroup*>(group)->sendMsgToGroupMember(msg, pSensor, pName);
        }
        else {
            pActor->receiveMessage(msg, pSensor, pActor->getSensor(pName));
        }
    }

    // Minor mismatch: Registers are switched around
    /* void sendMsgToAllLiveActor(u32 msg, LiveActor *pActor) {
        AllLiveActorGroup* group = getAllLiveActorGroup();
        for (int i = 0; i < group->mObjectCount; i++) {
            LiveActor* currentActor = group->getActor(i);
            if (isDead(currentActor) || currentActor == pActor) {
                continue;
            }
            HitSensor* pSensor2 = static_cast<LiveActor*>(getSceneObjHolder()->getObj(0x9))->getSensor(nullptr);
            currentActor->receiveMessage(msg, static_cast<LiveActor*>(getSceneObjHolder()->getObj(0x9))->getSensor(nullptr), pSensor2);
        }
    } */

    bool receiveItemShowMsg(u32 msg, HitSensor *pSensor1, HitSensor *pSensor2) {
        if (msg == 0x8a && isDead(pSensor2->mActor)) {
            pSensor2->mActor->makeActorAppeared();
            return true;
        }
        return false;
    }

    bool receiveItemHideMsg(u32 msg, HitSensor *pSensor1, HitSensor *pSensor2) {
        if (msg == 0x8b && !isDead(pSensor2->mActor)) {
            pSensor2->mActor->makeActorDead();
            return true;
        }
        return false;
    }

    HitSensor* getMessageSensor() {
        return static_cast<LiveActor*>(getSceneObjHolder()->getObj(9))->getSensor(nullptr);
    }

    HitSensor* getGroundSensor(const LiveActor *pActor) {
        return pActor->mBinder->groundInfo.mParentTriangle.mSensor;
    }

    HitSensor* getRoofSensor(const LiveActor *pActor) {
        return pActor->mBinder->roofInfo.mParentTriangle.mSensor;
    }

    HitSensor* getWallSensor(const LiveActor *pActor) {
        return pActor->mBinder->wallInfo.mParentTriangle.mSensor;
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

    bool isMsgPlayerKick(u32 msg) {
        return !(msg != 0x2b);
    }

    bool isMsgTurtleAttack(u32 msg) {
        return !(msg != 6);
    }

    bool isMsgFireBallAttack(u32 msg) {
        return !(msg != 8);
    }

    bool isMsgSearchlightAttack(u32 msg) {
        return !(msg != 9);
    }

    bool isMsgFreezeAttack(u32 msg) {
        return !(msg != 0xa);
    }

    bool isMsgInvinciblAttack(u32 msg) {
        return !(msg != 0xb);
    }

    bool isMsgInvalidHit(u32 msg) {
        return !(msg != 0x1d);
    }

    bool isMsgAutoRushBegin(u32 msg) {
        return !(msg != 0x92);
    }

    bool isMsgRushBegin(u32 msg) {
        return !(msg != 0x91);
    }

    bool isMsgUpdateBaseMtx(u32 msg) {
        return !(msg != 0xa1);
    }
    
    bool isMsgRushCancel(u32 msg) {
        return !(msg != 0x93);
    }

    bool isMsgIsRushTakeOver(u32 msg) {
        return !(msg != 0x98);
    }

    bool isMsgFloorTouch(u32 msg) {
        return !(msg != 0xb4);
    }

    bool isMsgWallTouch(u32 msg) {
        return !(msg != 0xb5);
    }

    bool isMsgCeilTouch(u32 msg) {
        return !(msg != 0xb6);
    }

    bool isMsgItemGet(u32 msg) {
        return !(msg != 0x87);
    }

    bool isMsgItemPull(u32 msg) {
        return !(msg != 0x89);
    }

    bool isMsgItemShow(u32 msg) {
        return !(msg != 0x8a);
    }

    bool isMsgItemHide(u32 msg) {
        return !(msg != 0x8b);
    }

    bool isMsgItemStartMove(u32 msg) {
        return !(msg != 0x8c);
    }

    bool isMsgItemEndMove(u32 msg) {
        return !(msg != 0x8d);
    }

    bool isMsgInhaleBlackHole(u32 msg) {
        return !(msg != 0x73);
    }

    bool isMsgEnemyAttack(u32 msg) {
        return !(msg != 0x53);
    }
    
    bool isMsgEnemyAttackFire(u32 msg) {
        return !(msg != 0x58);
    }

    bool isMsgEnemyAttackFireStrong(u32 msg) {
        return !(msg != 0x59);
    }

    bool isMsgEnemyAttackElectric(u32 msg) {
        return !(msg != 0x5a);
    }

    bool isMsgExplosionAttack(u32 msg) {
        return !(msg != 0x56);
    }

    bool isMsgToEnemyAttackBlow(u32 msg) {
        return !(msg != 0x62);
    }

    bool isMsgToEnemyAttackTrample(u32 msg) {
        return !(msg != 0x63);
    }

    bool isMsgToEnemyAttackShockWave(u32 msg) {
        return !(msg != 0x64);
    }

    bool isMsgSpinStormRange(u32 msg) {
        return !(msg != 0x33);
    }

    bool isMsgTutorialStart(u32 msg) {
        return !(msg != 0xe6);
    }

    bool isMsgTutorialNext(u32 msg) {
        return !(msg != 0xe7);
    }

    bool isMsgTutorialPrev(u32 msg) {
        return !(msg != 0xe8);
    }

    bool isMsgTutorialPass(u32 msg) {
        return !(msg != 0xe9);
    }

    bool isMsgTutorialOmit(u32 msg) {
        return !(msg != 0xeb);
    }

    bool isMsgRaceReady(u32 msg) {
        return !(msg != 0xec);
    }

    bool isMsgRaceStart(u32 msg) {
        return !(msg != 0xed);
    }

    bool isMsgRaceReset(u32 msg) {
        return !(msg != 0xef);
    }

    bool isMsgLockOnStarPieceShoot(u32 msg) {
        return !(msg != 0xe);
    }

    bool isMsgBallDashWall(u32 msg) {
        return !(msg != 0x39);
    }

    bool isMsgBallDashGround(u32 msg) {
        return !(msg != 0x3a);
    }

    bool isMsgStartPowerStarGet(u32 msg) {
        return !(msg != 0x74);
    }

    bool isMsgTouchPlantItem(u32 msg) {
        return !(msg != 0xf3);
    }

    bool isMsgHitmarkEmit(u32 msg) {
        return !(msg != 0x1c);
    }

    bool isMsgStarPieceAttack(u32 msg) {
        return !(msg != 0xc);
    }

    bool isMsgStarPieceReflect(u32 msg) {
        return !(msg != 0xd);
    }

    u32 getNumStarPieceGift(u32 msgId) {
        return msgId - 0xe;
    }

    bool tryForceKillIfMsgStartPowerStarGet(LiveActor *pActor, u32 msg) {
        if (msg == 0x74) {
            if (isExistEffectKeeper(pActor)) {
                forceDeleteEffectAll(pActor);
            }
            pActor->makeActorDead();
            return true;
        }
        return false;
    }

};

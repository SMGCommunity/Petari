#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/HitSensorInfo.hpp"
#include "Game/LiveActor/HitSensorKeeper.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/LiveActorGroupArray.hpp"
#include "Game/LiveActor/MessageSensorHolder.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

namespace MR {
    HitSensor* addHitSensor(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return pActor->mSensorKeeper->add(pName, type, groupSize, radius, pActor, rOffset);
    }

    HitSensor* addHitSensorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x61, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorTransferableBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x62, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x63, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x9, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x46, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorMapObjPress(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x76, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x47, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorMapObjMoveCollision(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x48, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x14, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorEnemySimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x15, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x16, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x5, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x7F, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorPush(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensor(pActor, pName, 0x80, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorPos(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        return pActor->mSensorKeeper->addPos(pName, type, groupSize, radius, pActor, pPos, rOffset);
    }

    HitSensor* addHitSensorPosBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        return addHitSensorPos(pActor, pName, 0x61, groupSize, radius, pPos, rOffset);
    }

    HitSensor* addHitSensorPosRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        return addHitSensorPos(pActor, pName, 0x9, groupSize, radius, pPos, rOffset);
    }

    HitSensor* addHitSensorPosMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        return addHitSensorPos(pActor, pName, 0x46, groupSize, radius, pPos, rOffset);
    }

    HitSensor* addHitSensorPosEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
        return addHitSensorPos(pActor, pName, 0x7F, groupSize, radius, pPos, rOffset);
    }

    HitSensor* addHitSensorMtx(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return pActor->mSensorKeeper->addMtx(pName, type, groupSize, radius, pActor, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x9, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x46, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x14, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x16, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x5, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorMtxAnimal(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, 0x7, groupSize, radius, pMtx, rOffset);
    }

    HitSensor* addHitSensorAtJoint(LiveActor* pActor, const char* pName, const char* pJointName, u32 type, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorMtx(pActor, pName, type, groupSize, radius, getJointMtx(pActor, pJointName), rOffset);
    }

    HitSensor* addHitSensorAtJointRide(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x9, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointMapObj(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x46, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointMapObjSimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x47, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointEnemy(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x14, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointEnemySimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x15, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointEnemyAttack(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x16, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointNpc(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x5, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorAtJointEye(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
        return addHitSensorAtJoint(pActor, pName, pJointName, 0x7F, groupSize, radius, rOffset);
    }

    HitSensor* addHitSensorCallback(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius) {
        return pActor->mSensorKeeper->addCallback(pName, type, groupSize, radius, pActor);
    }

    HitSensor* addHitSensorCallbackBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x61, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x63, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x9, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x46, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x47, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x14, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x16, groupSize, radius);
    }

    HitSensor* addHitSensorCallbackEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
        return addHitSensorCallback(pActor, pName, 0x7F, groupSize, radius);
    }

    HitSensor* addBodyMessageSensorReceiver(LiveActor* pActor) {
        return addBodyMessageSensor(pActor, 0x84);
    }

    HitSensor* addBodyMessageSensorMapObj(LiveActor* pActor) {
        return addBodyMessageSensor(pActor, 0x46);
    }

    HitSensor* addBodyMessageSensorMapObjPress(LiveActor* pActor) {
        return addBodyMessageSensor(pActor, 0x76);
    }

    HitSensor* addBodyMessageSensorMapObjMoveCollision(LiveActor* pActor) {
        return addBodyMessageSensor(pActor, 0x48);
    }

    HitSensor* addBodyMessageSensorEnemy(LiveActor* pActor) {
        return addBodyMessageSensor(pActor, 0x14);
    }

    HitSensor* addMessageSensor(LiveActor* pActor, const char* pName, u32 type) {
        return addHitSensor(pActor, pName, type, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    }

    HitSensor* addMessageSensorReceiver(LiveActor* pActor, const char* pName) {
        return addMessageSensor(pActor, pName, 0x84);
    }

    HitSensor* addMessageSensorMapObj(LiveActor* pActor, const char* pName) {
        return addMessageSensor(pActor, pName, 0x46);
    }

    HitSensor* addMessageSensorMapObjMoveCollision(LiveActor* pActor, const char* pName) {
        return addMessageSensor(pActor, pName, 0x48);
    }

    HitSensor* addMessageSensorEnemy(LiveActor* pActor, const char* pName) {
        return addMessageSensor(pActor, pName, 0x14);
    }

    bool tryUpdateHitSensorsAll(LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->update();

            return true;
        }

        return false;
    }

    void updateHitSensorsAll(LiveActor* pActor) {
        pActor->mSensorKeeper->update();
    }

    bool isSensorType(const HitSensor* pSensor, u32 type) {
        return pSensor->isType(type);
    }

    HitSensor* getSensorWithIndex(LiveActor* pActor, int index) {
        return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
    }

    HitSensor* getTaking(const LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            return pActor->mSensorKeeper->mTaking;
        }

        return nullptr;
    }

    HitSensor* getTaken(const LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            return pActor->mSensorKeeper->mTaken;
        }

        return nullptr;
    }

    void setSensorPos(HitSensor* pSensor, const TVec3f& rPos) {
        pSensor->mPosition.set(rPos);
    }

    void setSensorOffset(LiveActor* pActor, const char* pName, const TVec3f& rOffset) {
        pActor->mSensorKeeper->getSensorInfo(pName)->_C.setPS(rOffset);
    }

    void setSensorRadius(LiveActor* pActor, const char* pName, f32 radius) {
        pActor->mSensorKeeper->getSensorInfo(pName)->mSensor->mRadius = radius;
    }

    void setHitSensorApart(HitSensor* pSender, HitSensor* pReceiver) {
        if (getTaking(pSender->mActor) == pReceiver || getTaken(pReceiver->mActor) == pSender) {
            pSender->mActor->mSensorKeeper->mTaking = nullptr;
            pReceiver->mActor->mSensorKeeper->mTaken = nullptr;
        }
        else {
            pSender->mActor->mSensorKeeper->mTaken = nullptr;
            pReceiver->mActor->mSensorKeeper->mTaking = nullptr;
        }
    }

    void validateHitSensors(LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->validate();
        }
    }

    void invalidateHitSensors(LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->invalidate();
        }
    }

    void validateHitSensor(LiveActor* pActor, const char* pName) {
        pActor->mSensorKeeper->getSensor(pName)->validate();
    }

    void invalidateHitSensor(LiveActor* pActor, const char* pName) {
        pActor->mSensorKeeper->getSensor(pName)->invalidate();
    }

    bool isValidHitSensor(LiveActor* pActor, const char* pName) {
        return pActor->mSensorKeeper->getSensor(pName)->mValidByHost;
    }

    void clearHitSensors(LiveActor* pActor) {
        if (pActor->mSensorKeeper != nullptr) {
            pActor->mSensorKeeper->clear();
        }
    }

    HitSensor* getSensor(LiveActor* pActor, int index) {
        return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
    }

    LiveActor* getSensorHost(const HitSensor* pSensor) {
        return pSensor->mActor;
    }

    bool isSensor(const HitSensor* pSensor, const char* pName) {
        return pSensor == pSensor->mActor->getSensor(pName);
    }

    bool isSensorPlayer(const HitSensor* pSensor) {
        return pSensor->isType(0x1);
    }

    bool isSensorBinder(const HitSensor* pSensor) {
        return pSensor->isType(0x61);
    }

    bool isSensorRide(const HitSensor* pSensor) {
        if (pSensor->mType > 0x8 && pSensor->mType < 0x12) {
            return true;
        }

        return false;
    }

    bool isSensorPlayerOrRide(const HitSensor* pSensor) {
        return isSensorPlayer(pSensor) || isSensorRide(pSensor);
    }

    bool isSensorEnemy(const HitSensor* pSensor) {
        if (pSensor->mType > 0x13 && pSensor->mType < 0x44) {
            return true;
        }

        return false;
    }

    bool isSensorEnemyAttack(const HitSensor* pSensor) {
        return pSensor->isType(0x16);
    }

    bool isSensorNpc(const HitSensor* pSensor) {
        if (pSensor->mType > 0x4 && pSensor->mType < 0x6) {
            return true;
        }

        return false;
    }

    bool isSensorMapObj(const HitSensor* pSensor) {
        if (pSensor->mType > 0x45 && pSensor->mType < 0x5F) {
            return true;
        }

        return false;
    }

    bool isSensorAutoRush(const HitSensor* pSensor) {
        if (pSensor->mType > 0x60 && pSensor->mType < 0x6E) {
            return true;
        }

        return false;
    }

    bool isSensorRush(const HitSensor* pSensor) {
        if (pSensor->mType > 0x6F && pSensor->mType < 0x74) {
            return true;
        }

        return false;
    }

    bool isSensorPressObj(const HitSensor* pSensor) {
        if (pSensor->mType > 0x75 && pSensor->mType < 0x77) {
            return true;
        }

        return false;
    }

    bool isSensorEye(const HitSensor* pSensor) {
        return pSensor->isType(0x7F);
    }

    bool isSensorPush(const HitSensor* pSensor) {
        return pSensor->isType(0x80);
    }

    bool isSensorItem(const HitSensor* pSensor) {
        if (pSensor->isType(0x4A) || pSensor->isType(0x4B) || pSensor->isType(0x4D)) {
            return true;
        }

        return false;
    }

    bool tryGetItem(HitSensor* pSender, HitSensor* pReceiver) {
        return pReceiver->receiveMessage(0x87, pSender);
    }

    const TVec3f& getSensorPos(const HitSensor* pSensor) {
        return pSensor->mPosition;
    }

    void calcSensorDirection(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2) {
        TVec3f dir(pSensor2->mPosition);

        JMathInlineVEC::PSVECSubtract(&dir, &pSensor1->mPosition, &dir);
        pDir->set(dir);
    }

    void calcSensorDirectionNormalize(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2) {
        calcSensorDirection(pDir, pSensor1, pSensor2);
        normalizeOrZero(pDir);
    }

    void calcSensorHorizon(TVec3f* pHorizon, const TVec3f& rGravity, const HitSensor* pSensor1, const HitSensor* pSensor2) {
        TVec3f horizon(pSensor2->mPosition);

        JMathInlineVEC::PSVECSubtract(&horizon, &pSensor1->mPosition, &horizon);
        pHorizon->rejection(horizon, rGravity);
    }

    void calcSensorHorizonNormalize(TVec3f* pHorizon, const TVec3f& rGravity, const HitSensor* pSensor1, const HitSensor* pSensor2) {
        calcSensorHorizon(pHorizon, rGravity, pSensor1, pSensor2);
        normalizeOrZero(pHorizon);
    }

    HitSensor* getMessageSensor() {
        return getSceneObj<MessageSensorHolder*>(SceneObj_MessageSensorHolder)->getSensor(nullptr);
    }

    bool sendArbitraryMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
        return pReceiver->receiveMessage(msg, pSender);
    }

    bool sendMsgPush(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x29, pReceiver, pSender);
    }

    bool sendMsgPlayerTrample(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x2, pReceiver, pSender);
    }

    bool sendMsgPlayerPunch(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x1, pReceiver, pSender);
    }

    bool sendMsgJump(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x2C, pReceiver, pSender);
    }

    bool sendMsgTouchJump(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x2D, pReceiver, pSender);
    }

    bool sendMsgTaken(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x1F, pReceiver, pSender);
    }

    bool sendMsgKick(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x2B, pReceiver, pSender);
    }

    bool sendMsgAwayJump(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x2F, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
        TVec3f senderPos(pSender->mPosition);

        JMathInlineVEC::PSVECSubtract(&pReceiver->mPosition, &rDir, &pSender->mPosition);
        bool isSent = pReceiver->receiveMessage(msg, pSender);
        setSensorPos(pSender, senderPos);

        return isSent;
    }

    bool sendMsgEnemyAttackFlipWeak(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x4D, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlipWeakJump(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x4E, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlip(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x50, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlipToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
        return sendMsgEnemyAttackMsgToDir(0x50, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFlipJump(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x4F, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlipRot(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x51, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximum(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x52, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
        return sendMsgEnemyAttackMsgToDir(0x52, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttack(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x53, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackStrong(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x54, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackStrongToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
        return sendMsgEnemyAttackMsgToDir(0x54, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackFire(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x58, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFireStrong(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x59, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackElectric(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x5A, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackFreeze(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x5D, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackHeatBeam(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x5B, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackExplosion(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x56, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackCounterSpin(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x60, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackCounterHipDrop(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x61, pReceiver, pSender);
    }

    bool sendMsgLockOnStarPieceShoot(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0xE, pReceiver, pSender);
    }

    bool sendMsgStarPieceAttack(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0xC, pReceiver, pSender);
    }

    bool sendMsgStarPieceGift(HitSensor* pReceiver, HitSensor* pSender, u32 msg) {
        return sendArbitraryMsg(msg + 0xE, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackMaximum(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x55, pReceiver, pSender);
    }

    bool sendMsgEnemyAttackMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
        return sendMsgEnemyAttackMsgToDir(0x55, pReceiver, pSender, rDir);
    }

    bool sendMsgEnemyAttackToBindedSensor(LiveActor* pActor, HitSensor* pSender) {
        return sendMsgToBindedSensor(0x53, pActor, pSender);
    }

    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor* pActor, HitSensor* pSender) {
        return sendMsgToBindedSensor(0x56, pActor, pSender);
    }

    bool sendSimpleMsgToActor(u32 msg, LiveActor* pActor) {
        HitSensor* pReceiver = getMessageSensor();
        HitSensor* pSender = getMessageSensor();

        return pActor->receiveMessage(msg, pSender, pReceiver);
    }

    bool sendMsgToBindedSensor(u32 msg, LiveActor* pActor, HitSensor* pSender) {
        HitInfo* info[32];
        u32 size = pActor->mBinder->copyPlaneArrayAndSortingSensor(info, sizeof(info) / sizeof(*info));

        if (size == 0) {
            return false;
        }

        bool isSent = false;
        HitSensor* pReceiver = info[0]->mParentTriangle.mSensor;
        isSent |= pReceiver->receiveMessage(msg, pSender);

        for (int i = 1; i < size; i++) {
            if (info[i]->mParentTriangle.mSensor == pReceiver) {
                continue;
            }

            pReceiver = info[i]->mParentTriangle.mSensor;
            isSent |= pReceiver->receiveMessage(msg, pSender);
        }

        return isSent;
    }

    bool sendMsgToBindedSensor(u32 msg, HitSensor* pSender) {
        return sendMsgToBindedSensor(msg, pSender->mActor, pSender);
    }

    bool sendMsgToGroundSensor(u32 msg, HitSensor* pSender) {
        if (!isBindedGround(pSender->mActor)) {
            return false;
        }

        // FIXME: getGroundSensor should not be inlined.
        return getGroundSensor(pSender->mActor)->receiveMessage(msg, pSender);
    }

    bool sendMsgToWallSensor(u32 msg, HitSensor* pSender) {
        if (!isBindedWall(pSender->mActor)) {
            return false;
        }

        // FIXME: getWallSensor should not be inlined.
        return getWallSensor(pSender->mActor)->receiveMessage(msg, pSender);
    }

    bool sendMsgStartDemo(LiveActor* pActor) {
        return sendSimpleMsgToActor(0x6F, pActor);
    }

    bool sendMsgToEnemyAttackBlow(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x62, pReceiver, pSender);
    }

    bool sendMsgToEnemyAttackTrample(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x63, pReceiver, pSender);
    }

    bool sendMsgToEnemyAttackBlowOrTrample(HitSensor* pReceiver, HitSensor* pSender, f32 ratio) {
        TVec3f dir(pReceiver->mPosition);

        JMathInlineVEC::PSVECSubtract(&dir, &pSender->mPosition, &dir);
        normalizeOrZero(&dir);

        if (ratio < pSender->mActor->mGravity.dot(dir)) {
            return sendMsgToEnemyAttackTrample(pReceiver, pSender);
        }
        else {
            return sendMsgToEnemyAttackBlow(pReceiver, pSender);
        }
    }

    bool sendMsgToEnemyAttackShockWave(HitSensor* pReceiver, HitSensor* pSender) {
        return sendArbitraryMsg(0x64, pReceiver, pSender);
    }

    void sendMsgToAllLiveActor(u32 msg, LiveActor* pActor) {
        AllLiveActorGroup* pGroup = getAllLiveActorGroup();

        for (int i = 0; i < pGroup->getObjectCount(); i++) {
            LiveActor* pGroupActor = pGroup->getActor(i);

            if (isDead(pGroupActor)) {
                continue;
            }

            if (pGroupActor == pActor) {
                continue;
            }

            pGroupActor->receiveMessage(msg, getMessageSensor(), getMessageSensor());
        }
    }

    void sendMsgToGroupMember(u32 msg, LiveActor* pActor, HitSensor* pSender, const char* pName) {
        MsgSharedGroup* pGroup = static_cast<MsgSharedGroup*>(getGroupFromArray(pActor));

        if (pGroup != nullptr) {
            pGroup->sendMsgToGroupMember(msg, pSender, pName);
        }
        else {
            pActor->receiveMessage(msg, pSender, pActor->getSensor(pName));
        }
    }

    void sendMsgExplosionToNearActor(HitSensor* pSender, f32 distance) {
        HitSensor* pReceiver;

        for (u16 i = 0; i < pSender->mSensorCount; i++) {
            pReceiver = pSender->mSensors[i];

            if (distance < 0.0f || isNear(pSender, pReceiver, distance)) {
                sendArbitraryMsg(0x56, pReceiver, pSender);
            }
        }
    }

    bool isInSpinStormRange(u32 msg, HitSensor* pSender, HitSensor* pReceiver, f32 range) {
        if (msg != 0x33) {
            return false;
        }

        // NOTE: Double negative?
        bool isFar = !isNear(pSender, pReceiver, range);

        return !isFar;
    }

    bool receiveItemShowMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        if (msg == 0x8A && isDead(pReceiver->mActor)) {
            pReceiver->mActor->makeActorAppeared();

            return true;
        }

        return false;
    }

    bool receiveItemHideMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        if (msg == 0x8B && !isDead(pReceiver->mActor)) {
            pReceiver->mActor->makeActorDead();

            return true;
        }

        return false;
    }

    HitSensor* getGroundSensor(const LiveActor* pActor) {
        return pActor->mBinder->mGroundInfo.mParentTriangle.mSensor;
    }

    HitSensor* getRoofSensor(const LiveActor* pActor) {
        return pActor->mBinder->mRoofInfo.mParentTriangle.mSensor;
    }

    HitSensor* getWallSensor(const LiveActor* pActor) {
        return pActor->mBinder->mWallInfo.mParentTriangle.mSensor;
    }

    bool isMsgPlayerHitAll(u32 msg) {
        return msg == 0x1
            || msg == 0x5
            || msg == 0x6
            || msg == 0x8
            || msg == 0xA
            || msg == 0xB;
    }

    bool isMsgPlayerSpinAttack(u32 msg) {
        return msg == 0x1;
    }

    bool isMsgPlayerTrample(u32 msg) {
        return msg == 0x2;
    }

    bool isMsgPlayerHipDrop(u32 msg) {
        return msg == 0x3;
    }

    bool isMsgPlayerHipDropFloor(u32 msg) {
        return msg == 0x4;
    }

    bool isMsgPlayerUpperPunch(u32 msg) {
        return msg == 0x5;
    }

    bool isMsgPlayerKick(u32 msg) {
        return msg == 0x2B;
    }

    bool isMsgJetTurtleAttack(u32 msg) {
        return msg == 0x6;
    }

    bool isMsgFireBallAttack(u32 msg) {
        return msg == 0x8;
    }

    bool isMsgSearchlightAttack(u32 msg) {
        return msg == 0x9;
    }

    bool isMsgFreezeAttack(u32 msg) {
        return msg == 0xA;
    }

    bool isMsgInvincibleAttack(u32 msg) {
        return msg == 0xB;
    }

    bool isMsgInvalidHit(u32 msg) {
        return msg == 0x1D;
    }

    bool isMsgAutoRushBegin(u32 msg) {
        return msg == 0x92;
    }

    bool isMsgRushBegin(u32 msg) {
        return msg == 0x91;
    }

    bool isMsgUpdateBaseMtx(u32 msg) {
        return msg == 0xA1;
    }

    bool isMsgRushCancel(u32 msg) {
        return msg == 0x93;
    }

    bool isMsgIsRushTakeOver(u32 msg) {
        return msg == 0x98;
    }

    bool isMsgFloorTouch(u32 msg) {
        return msg == 0xB4;
    }

    bool isMsgWallTouch(u32 msg) {
        return msg == 0xB5;
    }

    bool isMsgCeilTouch(u32 msg) {
        return msg == 0xB6;
    }

    bool isMsgItemGet(u32 msg) {
        return msg == 0x87;
    }

    bool isMsgItemPull(u32 msg) {
        return msg == 0x89;
    }

    bool isMsgItemShow(u32 msg) {
        return msg == 0x8A;
    }

    bool isMsgItemHide(u32 msg) {
        return msg == 0x8B;
    }

    bool isMsgItemStartMove(u32 msg) {
        return msg == 0x8C;
    }

    bool isMsgItemEndMove(u32 msg) {
        return msg == 0x8D;
    }

    bool isMsgInhaleBlackHole(u32 msg) {
        return msg == 0x73;
    }

    bool isMsgEnemyAttack(u32 msg) {
        return msg == 0x53;
    }

    bool isMsgEnemyAttackFire(u32 msg) {
        return msg == 0x58;
    }

    bool isMsgEnemyAttackFireStrong(u32 msg) {
        return msg == 0x59;
    }

    bool isMsgEnemyAttackElectric(u32 msg) {
        return msg == 0x5A;
    }

    bool isMsgExplosionAttack(u32 msg) {
        return msg == 0x56;
    }

    bool isMsgToEnemyAttackBlow(u32 msg) {
        return msg == 0x62;
    }

    bool isMsgToEnemyAttackTrample(u32 msg) {
        return msg == 0x63;
    }

    bool isMsgToEnemyAttackShockWave(u32 msg) {
        return msg == 0x64;
    }

    bool isMsgSpinStormRange(u32 msg) {
        return msg == 0x33;
    }

    bool isMsgTutorialStart(u32 msg) {
        return msg == 0xE6;
    }

    bool isMsgTutorialNext(u32 msg) {
        return msg == 0xE7;
    }

    bool isMsgTutorialPrev(u32 msg) {
        return msg == 0xE8;
    }

    bool isMsgTutorialPass(u32 msg) {
        return msg == 0xE9;
    }

    bool isMsgTutorialOmit(u32 msg) {
        return msg == 0xEB;
    }

    bool isMsgRaceReady(u32 msg) {
        return msg == 0xEC;
    }

    bool isMsgRaceStart(u32 msg) {
        return msg == 0xED;
    }

    bool isMsgRaceReset(u32 msg) {
        return msg == 0xEF;
    }

    bool isMsgLockOnStarPieceShoot(u32 msg) {
        return msg == 0xE;
    }

    bool isMsgBallDashWall(u32 msg) {
        return msg == 0x39;
    }

    bool isMsgBallDashGround(u32 msg) {
        return msg == 0x3A;
    }

    bool isMsgStartPowerStarGet(u32 msg) {
        return msg == 0x74;
    }

    bool isMsgTouchPlantItem(u32 msg) {
        return msg == 0xF3;
    }

    bool isMsgHitmarkEmit(u32 msg) {
        return msg == 0x1C;
    }

    bool isMsgStarPieceAttack(u32 msg) {
        return msg == 0xC;
    }

    bool isMsgStarPieceReflect(u32 msg) {
        return msg == 0xD;
    }

    bool isMsgStarPieceGift(u32 msg) {
        return msg >= 0xF && msg <= 0x19;
    }

    s32 getNumStarPieceGift(u32 msg) {
        return msg - 14;
    }

    void calcPosBetweenSensors(TVec3f* pPos, const HitSensor* pSensor1, const HitSensor* pSensor2, f32 offset) {
        TVec3f dir(pSensor2->mPosition);

        JMathInlineVEC::PSVECSubtract(&dir, &pSensor1->mPosition, &dir);
        normalizeOrZero(&dir);

        f32 dist = PSVECDistance(&pSensor1->mPosition, &pSensor2->mPosition);
        f32 radius2 = pSensor2->mRadius;
        f32 radius1 = pSensor1->mRadius;
        f32 value = (radius1 + radius2 - dist) / 2.0f;

        pPos->set(dir);
        pPos->x *= value + pSensor1->mRadius + offset;
        pPos->y *= value + pSensor1->mRadius + offset;
        pPos->z *= value + pSensor1->mRadius + offset;

        JMathInlineVEC::PSVECAdd2(pPos, &pSensor1->mPosition, pPos);
    }

    bool tryForceKillIfMsgStartPowerStarGet(LiveActor* pActor, u32 msg) {
        if (msg == 0x74) {
            if (isExistEffectKeeper(pActor)) {
                forceDeleteEffectAll(pActor);
            }

            pActor->makeActorDead();

            return true;
        }

        return false;
    }

    HitSensor* addBodyMessageSensor(LiveActor* pActor, u32 type) {
        return addHitSensor(pActor, "body", type, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
    }
};

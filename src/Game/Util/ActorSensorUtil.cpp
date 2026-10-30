#include "Game/Util/ActorSensorUtil.hpp"
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
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"

HitSensor* MR::addHitSensor(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return pActor->mSensorKeeper->add(pName, type, groupSize, radius, pActor, rOffset);
}

HitSensor* MR::addHitSensorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_BINDER, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorTransferableBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_TRANSFERABLE_BINDER, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_PRIOR_BINDER, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_RIDE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_MAP_OBJ, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorMapObjPress(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_MAP_OBJ_PRESS, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_MAP_OBJ_SIMPLE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorMapObjMoveCollision(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_MAP_OBJ_MOVE_COLLISION, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_ENEMY, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorEnemySimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_ENEMY_SIMPLE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_ENEMY_ATTACK, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_NPC, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_EYE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorPush(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f& rOffset) {
    return addHitSensor(pActor, pName, ATYPE_PUSH, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorPos(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
    return pActor->mSensorKeeper->addPos(pName, type, groupSize, radius, pActor, pPos, rOffset);
}

HitSensor* MR::addHitSensorPosBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
    return addHitSensorPos(pActor, pName, ATYPE_BINDER, groupSize, radius, pPos, rOffset);
}

HitSensor* MR::addHitSensorPosRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
    return addHitSensorPos(pActor, pName, ATYPE_RIDE, groupSize, radius, pPos, rOffset);
}

HitSensor* MR::addHitSensorPosMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
    return addHitSensorPos(pActor, pName, ATYPE_MAP_OBJ, groupSize, radius, pPos, rOffset);
}

HitSensor* MR::addHitSensorPosEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, const TVec3f* pPos, const TVec3f& rOffset) {
    return addHitSensorPos(pActor, pName, ATYPE_EYE, groupSize, radius, pPos, rOffset);
}

HitSensor* MR::addHitSensorMtx(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return pActor->mSensorKeeper->addMtx(pName, type, groupSize, radius, pActor, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_RIDE, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_MAP_OBJ, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_ENEMY, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_ENEMY_ATTACK, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxNpc(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_NPC, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorMtxAnimal(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius, MtxPtr pMtx, const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, ATYPE_ANIMAL, groupSize, radius, pMtx, rOffset);
}

HitSensor* MR::addHitSensorAtJoint(LiveActor* pActor, const char* pName, const char* pJointName, u32 type, u16 groupSize, f32 radius,
                                   const TVec3f& rOffset) {
    return addHitSensorMtx(pActor, pName, type, groupSize, radius, getJointMtx(pActor, pJointName), rOffset);
}

HitSensor* MR::addHitSensorAtJointRide(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                       const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_RIDE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointMapObj(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                         const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_MAP_OBJ, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointMapObjSimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                               const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_MAP_OBJ_SIMPLE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointEnemy(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                        const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_ENEMY, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointEnemySimple(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                              const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_ENEMY_SIMPLE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointEnemyAttack(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                              const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_ENEMY_ATTACK, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointNpc(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                      const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_NPC, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorAtJointEye(LiveActor* pActor, const char* pName, const char* pJointName, u16 groupSize, f32 radius,
                                      const TVec3f& rOffset) {
    return addHitSensorAtJoint(pActor, pName, pJointName, ATYPE_EYE, groupSize, radius, rOffset);
}

HitSensor* MR::addHitSensorCallback(LiveActor* pActor, const char* pName, u32 type, u16 groupSize, f32 radius) {
    return pActor->mSensorKeeper->addCallback(pName, type, groupSize, radius, pActor);
}

HitSensor* MR::addHitSensorCallbackBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_BINDER, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackPriorBinder(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_PRIOR_BINDER, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackRide(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_RIDE, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackMapObj(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_MAP_OBJ, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackMapObjSimple(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_MAP_OBJ_SIMPLE, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackEnemy(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_ENEMY, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackEnemyAttack(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_ENEMY_ATTACK, groupSize, radius);
}

HitSensor* MR::addHitSensorCallbackEye(LiveActor* pActor, const char* pName, u16 groupSize, f32 radius) {
    return addHitSensorCallback(pActor, pName, ATYPE_EYE, groupSize, radius);
}

HitSensor* MR::addBodyMessageSensorReceiver(LiveActor* pActor) {
    return addBodyMessageSensor(pActor, ATYPE_RECEIVER);
}

HitSensor* MR::addBodyMessageSensorMapObj(LiveActor* pActor) {
    return addBodyMessageSensor(pActor, ATYPE_MAP_OBJ);
}

HitSensor* MR::addBodyMessageSensorMapObjPress(LiveActor* pActor) {
    return addBodyMessageSensor(pActor, ATYPE_MAP_OBJ_PRESS);
}

HitSensor* MR::addBodyMessageSensorMapObjMoveCollision(LiveActor* pActor) {
    return addBodyMessageSensor(pActor, ATYPE_MAP_OBJ_MOVE_COLLISION);
}

HitSensor* MR::addBodyMessageSensorEnemy(LiveActor* pActor) {
    return addBodyMessageSensor(pActor, ATYPE_ENEMY);
}

HitSensor* MR::addMessageSensor(LiveActor* pActor, const char* pName, u32 type) {
    return addHitSensor(pActor, pName, type, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

HitSensor* MR::addMessageSensorReceiver(LiveActor* pActor, const char* pName) {
    return addMessageSensor(pActor, pName, ATYPE_RECEIVER);
}

HitSensor* MR::addMessageSensorMapObj(LiveActor* pActor, const char* pName) {
    return addMessageSensor(pActor, pName, ATYPE_MAP_OBJ);
}

HitSensor* MR::addMessageSensorMapObjMoveCollision(LiveActor* pActor, const char* pName) {
    return addMessageSensor(pActor, pName, ATYPE_MAP_OBJ_MOVE_COLLISION);
}

HitSensor* MR::addMessageSensorEnemy(LiveActor* pActor, const char* pName) {
    return addMessageSensor(pActor, pName, ATYPE_ENEMY);
}

bool MR::tryUpdateHitSensorsAll(LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        updateHitSensorsAll(pActor);

        return true;
    }

    return false;
}

void MR::updateHitSensorsAll(LiveActor* pActor) {
    pActor->mSensorKeeper->update();
}

bool MR::isSensorType(const HitSensor* pSensor, u32 type) {
    return pSensor->isType(type);
}

HitSensor* MR::getSensorWithIndex(LiveActor* pActor, int index) {
    return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
}

HitSensor* MR::getTaking(const LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        return pActor->mSensorKeeper->mTaking;
    }

    return nullptr;
}

HitSensor* MR::getTaken(const LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        return pActor->mSensorKeeper->mTaken;
    }

    return nullptr;
}

void MR::setSensorPos(HitSensor* pSensor, const TVec3f& rPos) {
    pSensor->mPosition.set(rPos);
}

void MR::setSensorOffset(LiveActor* pActor, const char* pName, const TVec3f& rOffset) {
    pActor->mSensorKeeper->getSensorInfo(pName)->setOffset(rOffset);
}

void MR::setSensorRadius(LiveActor* pActor, const char* pName, f32 radius) {
    pActor->mSensorKeeper->getSensorInfo(pName)->mSensor->mRadius = radius;
}

void MR::setHitSensorApart(HitSensor* pSender, HitSensor* pReceiver) {
    if (getTaking(getSensorHost(pSender)) == pReceiver || getTaken(getSensorHost(pReceiver)) == pSender) {
        getSensorHost(pSender)->mSensorKeeper->mTaking = nullptr;
        getSensorHost(pReceiver)->mSensorKeeper->mTaken = nullptr;
    } else {
        getSensorHost(pSender)->mSensorKeeper->mTaken = nullptr;
        getSensorHost(pReceiver)->mSensorKeeper->mTaking = nullptr;
    }
}

void MR::validateHitSensors(LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        pActor->mSensorKeeper->validate();
    }
}

void MR::invalidateHitSensors(LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        pActor->mSensorKeeper->invalidate();
    }
}

void MR::validateHitSensor(LiveActor* pActor, const char* pName) {
    pActor->mSensorKeeper->getSensor(pName)->validate();
}

void MR::invalidateHitSensor(LiveActor* pActor, const char* pName) {
    pActor->mSensorKeeper->getSensor(pName)->invalidate();
}

bool MR::isValidHitSensor(LiveActor* pActor, const char* pName) {
    return pActor->mSensorKeeper->getSensor(pName)->mValidByHost;
}

void MR::clearHitSensors(LiveActor* pActor) {
    if (pActor->mSensorKeeper != nullptr) {
        pActor->mSensorKeeper->clear();
    }
}

HitSensor* MR::getSensor(LiveActor* pActor, int index) {
    return pActor->mSensorKeeper->getNthSensorInfo(index)->mSensor;
}

LiveActor* MR::getSensorHost(const HitSensor* pSensor) {
    return pSensor->mHost;
}

bool MR::isSensor(const HitSensor* pSensor, const char* pName) {
    return pSensor == getSensorHost(pSensor)->getSensor(pName);
}

bool MR::isSensorPlayer(const HitSensor* pSensor) {
    return pSensor->isType(ATYPE_PLAYER);
}

bool MR::isSensorBinder(const HitSensor* pSensor) {
    return pSensor->isType(ATYPE_BINDER);
}

bool MR::isSensorRide(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_RIDE_START && pSensor->mType < ATYPE_RIDE_END) {
        return true;
    }

    return false;
}

bool MR::isSensorPlayerOrRide(const HitSensor* pSensor) {
    return isSensorPlayer(pSensor) || isSensorRide(pSensor);
}

bool MR::isSensorEnemy(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_ENEMY_START && pSensor->mType < ATYPE_ENEMY_END) {
        return true;
    }

    return false;
}

bool MR::isSensorEnemyAttack(const HitSensor* pSensor) {
    return pSensor->isType(ATYPE_ENEMY_ATTACK);
}

bool MR::isSensorNpc(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_NPC_START && pSensor->mType < ATYPE_NPC_END) {
        return true;
    }

    return false;
}

bool MR::isSensorMapObj(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_MAPOBJ_START && pSensor->mType < ATYPE_MAPOBJ_END) {
        return true;
    }

    return false;
}

bool MR::isSensorAutoRush(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_AUTO_RUSH_OBJ_START && pSensor->mType < ATYPE_AUTO_RUSH_OBJ_END) {
        return true;
    }

    return false;
}

bool MR::isSensorRush(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_RUSH_OBJ_START && pSensor->mType < ATYPE_RUSH_OBJ_END) {
        return true;
    }

    return false;
}

bool MR::isSensorPressObj(const HitSensor* pSensor) {
    if (pSensor->mType > ATYPE_PRESS_OBJ_START && pSensor->mType < ATYPE_PRESS_OBJ_END) {
        return true;
    }

    return false;
}

bool MR::isSensorEye(const HitSensor* pSensor) {
    return pSensor->isType(ATYPE_EYE);
}

bool MR::isSensorPush(const HitSensor* pSensor) {
    return pSensor->isType(ATYPE_PUSH);
}

bool MR::isSensorItem(const HitSensor* pSensor) {
    if (pSensor->isType(ATYPE_COIN) || pSensor->isType(ATYPE_COIN_RED) || pSensor->isType(ATYPE_KINOKO_ONEUP)) {
        return true;
    }

    return false;
}

bool MR::tryGetItem(HitSensor* pSender, HitSensor* pReceiver) {
    return pReceiver->receiveMessage(ACTMES_ITEM_GET, pSender);
}

const TVec3f& MR::getSensorPos(const HitSensor* pSensor) {
    return pSensor->mPosition;
}

void MR::calcSensorDirection(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2) {
    TVec3f dir = pSensor2->mPosition - pSensor1->mPosition;
    pDir->set(dir);
}

void MR::calcSensorDirectionNormalize(TVec3f* pDir, const HitSensor* pSensor1, const HitSensor* pSensor2) {
    calcSensorDirection(pDir, pSensor1, pSensor2);
    normalizeOrZero(pDir);
}

void MR::calcSensorHorizon(TVec3f* pHorizon, const TVec3f& rGravity, const HitSensor* pSensor1, const HitSensor* pSensor2) {
    TVec3f horizon = pSensor2->mPosition - pSensor1->mPosition;
    pHorizon->killElement(horizon, rGravity);
}

void MR::calcSensorHorizonNormalize(TVec3f* pHorizon, const TVec3f& rGravity, const HitSensor* pSensor1, const HitSensor* pSensor2) {
    calcSensorHorizon(pHorizon, rGravity, pSensor1, pSensor2);
    normalizeOrZero(pHorizon);
}

HitSensor* MR::getMessageSensor() {
    return getSceneObj< MessageSensorHolder >(SceneObj_MessageSensorHolder)->getSensor(nullptr);
}

bool MR::sendArbitraryMsg(u32 msg, HitSensor* pReceiver, HitSensor* pSender) {
    return pReceiver->receiveMessage(msg, pSender);
}

bool MR::sendMsgPush(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_PUSH, pReceiver, pSender);
}

bool MR::sendMsgPlayerTrample(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_PLAYER_TRAMPLE, pReceiver, pSender);
}

bool MR::sendMsgPlayerPunch(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_PLAYER_PUNCH, pReceiver, pSender);
}

bool MR::sendMsgJump(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_JUMP, pReceiver, pSender);
}

bool MR::sendMsgTouchJump(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_TOUCHJUMP, pReceiver, pSender);
}

bool MR::sendMsgTaken(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_TAKEN, pReceiver, pSender);
}

bool MR::sendMsgKick(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_KICK, pReceiver, pSender);
}

bool MR::sendMsgAwayJump(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_AWAYJUMP, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackMsgToDir(u32 msg, HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
    TVec3f senderPos = pSender->mPosition;
    pSender->mPosition.sub(pReceiver->mPosition, rDir);

    bool isSent = pReceiver->receiveMessage(msg, pSender);
    setSensorPos(pSender, senderPos);

    return isSent;
}

bool MR::sendMsgEnemyAttackFlipWeak(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_WEAK, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlipWeakJump(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_WEAK_JUMP, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlip(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlipToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
    return sendMsgEnemyAttackMsgToDir(ACTMES_ENEMY_ATTACK_FLIP, pReceiver, pSender, rDir);
}

bool MR::sendMsgEnemyAttackFlipJump(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_JUMP, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlipRot(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_ROT, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlipMaximum(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFlipMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
    return sendMsgEnemyAttackMsgToDir(ACTMES_ENEMY_ATTACK_FLIP_MAXIMUM, pReceiver, pSender, rDir);
}

bool MR::sendMsgEnemyAttack(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackStrong(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_STRONG, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackStrongToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
    return sendMsgEnemyAttackMsgToDir(ACTMES_ENEMY_ATTACK_STRONG, pReceiver, pSender, rDir);
}

bool MR::sendMsgEnemyAttackFire(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FIRERUN, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFireStrong(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FIRERUN_STRONG, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackElectric(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_ELECTRIC, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackFreeze(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_FREEZE, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackHeatBeam(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_HEATBEAM, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackExplosion(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_EXPLOSION, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackCounterSpin(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_COUNTER_SPIN, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackCounterHipDrop(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_COUNTER_HIPDROP, pReceiver, pSender);
}

bool MR::sendMsgLockOnStarPieceShoot(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_IS_LOCKON_STAR_PIECE_SHOOT, pReceiver, pSender);
}

bool MR::sendMsgStarPieceAttack(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_STAR_PIECE_ATTACK, pReceiver, pSender);
}

bool MR::sendMsgStarPieceGift(HitSensor* pReceiver, HitSensor* pSender, u32 num) {
    return sendArbitraryMsg((num + ACTMES_STAR_PIECE_GIFT) - 1, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackMaximum(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_ENEMY_ATTACK_MAXIMUM, pReceiver, pSender);
}

bool MR::sendMsgEnemyAttackMaximumToDir(HitSensor* pReceiver, HitSensor* pSender, const TVec3f& rDir) {
    return sendMsgEnemyAttackMsgToDir(ACTMES_ENEMY_ATTACK_MAXIMUM, pReceiver, pSender, rDir);
}

bool MR::sendMsgEnemyAttackToBindedSensor(LiveActor* pActor, HitSensor* pSender) {
    return sendMsgToBindedSensor(ACTMES_ENEMY_ATTACK, pActor, pSender);
}

bool MR::sendMsgEnemyAttackExplosionToBindedSensor(LiveActor* pActor, HitSensor* pSender) {
    return sendMsgToBindedSensor(ACTMES_ENEMY_ATTACK_EXPLOSION, pActor, pSender);
}

bool MR::sendSimpleMsgToActor(u32 msg, LiveActor* pActor) {
    HitSensor* pReceiver = getMessageSensor();
    HitSensor* pSender = getMessageSensor();

    return pActor->receiveMessage(msg, pSender, pReceiver);
}

bool MR::sendMsgToBindedSensor(u32 msg, LiveActor* pActor, HitSensor* pSender) {
    HitInfo* info[32];
    u32 size = pActor->mBinder->copyPlaneArrayAndSortingSensor(info, ARRAY_SIZE(info));

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

bool MR::sendMsgToBindedSensor(u32 msg, HitSensor* pSender) {
    return sendMsgToBindedSensor(msg, getSensorHost(pSender), pSender);
}

bool MR::sendMsgToGroundSensor(u32 msg, HitSensor* pSender) {
    if (!isBindedGround(getSensorHost(pSender))) {
        return false;
    }

    return getGroundSensor(getSensorHost(pSender))->receiveMessage(msg, pSender);
}

bool MR::sendMsgToWallSensor(u32 msg, HitSensor* pSender) {
    if (!isBindedWall(getSensorHost(pSender))) {
        return false;
    }

    return getWallSensor(getSensorHost(pSender))->receiveMessage(msg, pSender);
}

bool MR::sendMsgStartDemo(LiveActor* pActor) {
    return sendSimpleMsgToActor(ACTMES_START_DEMO, pActor);
}

bool MR::sendMsgToEnemyAttackBlow(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_TO_ENEMY_ATTACK_BLOW, pReceiver, pSender);
}

bool MR::sendMsgToEnemyAttackTrample(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_TO_ENEMY_ATTACK_TRAMPLE, pReceiver, pSender);
}

bool MR::sendMsgToEnemyAttackBlowOrTrample(HitSensor* pReceiver, HitSensor* pSender, f32 ratio) {
    TVec3f dir = pReceiver->mPosition - pSender->mPosition;
    normalizeOrZero(&dir);

    if (ratio < getSensorHost(pSender)->mGravity.dot(dir)) {
        return sendMsgToEnemyAttackTrample(pReceiver, pSender);
    } else {
        return sendMsgToEnemyAttackBlow(pReceiver, pSender);
    }
}

bool MR::sendMsgToEnemyAttackShockWave(HitSensor* pReceiver, HitSensor* pSender) {
    return sendArbitraryMsg(ACTMES_TO_ENEMY_ATTACK_SHOCK_WAVE, pReceiver, pSender);
}

void MR::sendMsgToAllLiveActor(u32 msg, LiveActor* pActor) {
    AllLiveActorGroup* pGroup = getAllLiveActorGroup();

    for (int i = 0; i < pGroup->getObjNum(); i++) {
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

void MR::sendMsgToGroupMember(u32 msg, LiveActor* pActor, HitSensor* pSender, const char* pName) {
    MsgSharedGroup* pGroup = static_cast< MsgSharedGroup* >(getGroupFromArray(pActor));

    if (pGroup != nullptr) {
        pGroup->sendMsgToGroupMember(msg, pSender, pName);
    } else {
        pActor->receiveMessage(msg, pSender, pActor->getSensor(pName));
    }
}

void MR::sendMsgExplosionToNearActor(HitSensor* pSender, f32 distance) {
    HitSensor* pReceiver;

    for (u16 i = 0; i < pSender->mSensorCount; i++) {
        pReceiver = pSender->mSensors[i];

        if (distance < 0.0f || isNear(pSender, pReceiver, distance)) {
            sendArbitraryMsg(ACTMES_ENEMY_ATTACK_EXPLOSION, pReceiver, pSender);
        }
    }
}

bool MR::isInSpinStormRange(u32 msg, HitSensor* pSender, HitSensor* pReceiver, f32 range) {
    if (msg != ACTMES_SPIN_STORM_RANGE) {
        return false;
    }

    // NOTE: Double negative?
    bool isFar = !isNear(pSender, pReceiver, range);

    return !isFar;
}

bool MR::receiveItemShowMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_ITEM_SHOW && isDead(getSensorHost(pReceiver))) {
        getSensorHost(pReceiver)->makeActorAppeared();

        return true;
    }

    return false;
}

bool MR::receiveItemHideMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_ITEM_HIDE && !isDead(getSensorHost(pReceiver))) {
        getSensorHost(pReceiver)->makeActorDead();

        return true;
    }

    return false;
}

HitSensor* MR::getGroundSensor(const LiveActor* pActor) {
    return pActor->mBinder->mGroundInfo.mParentTriangle.mSensor;
}

HitSensor* MR::getRoofSensor(const LiveActor* pActor) {
    return pActor->mBinder->mRoofInfo.mParentTriangle.mSensor;
}

HitSensor* MR::getWallSensor(const LiveActor* pActor) {
    return pActor->mBinder->mWallInfo.mParentTriangle.mSensor;
}

bool MR::isMsgPlayerHitAll(u32 msg) {
    return msg == ACTMES_PLAYER_PUNCH || msg == ACTMES_PLAYER_UPPER_PUNCH || msg == ACTMES_JET_TURTLE_ATTACK || msg == ACTMES_FIREBALL_ATTACK ||
           msg == ACTMES_FREEZE_ATTACK || msg == ACTMES_INVINCIBLE_ATTACK;
}

bool MR::isMsgPlayerSpinAttack(u32 msg) {
    return msg == ACTMES_PLAYER_PUNCH;
}

bool MR::isMsgPlayerTrample(u32 msg) {
    return msg == ACTMES_PLAYER_TRAMPLE;
}

bool MR::isMsgPlayerHipDrop(u32 msg) {
    return msg == ACTMES_PLAYER_HIP_DROP;
}

bool MR::isMsgPlayerHipDropFloor(u32 msg) {
    return msg == ACTMES_PLAYER_HIP_DROP_FLOOR;
}

bool MR::isMsgPlayerUpperPunch(u32 msg) {
    return msg == ACTMES_PLAYER_UPPER_PUNCH;
}

bool MR::isMsgPlayerKick(u32 msg) {
    return msg == ACTMES_KICK;
}

bool MR::isMsgJetTurtleAttack(u32 msg) {
    return msg == ACTMES_JET_TURTLE_ATTACK;
}

bool MR::isMsgFireBallAttack(u32 msg) {
    return msg == ACTMES_FIREBALL_ATTACK;
}

bool MR::isMsgSearchlightAttack(u32 msg) {
    return msg == ACTMES_SEARCHLIGHT_ATTACK;
}

bool MR::isMsgFreezeAttack(u32 msg) {
    return msg == ACTMES_FREEZE_ATTACK;
}

bool MR::isMsgInvincibleAttack(u32 msg) {
    return msg == ACTMES_INVINCIBLE_ATTACK;
}

bool MR::isMsgInvalidHit(u32 msg) {
    return msg == ACTMES_INVALID_HIT;
}

bool MR::isMsgAutoRushBegin(u32 msg) {
    return msg == ACTMES_AUTORUSH_BEGIN;
}

bool MR::isMsgRushBegin(u32 msg) {
    return msg == ACTMES_RUSH_BEGIN;
}

bool MR::isMsgUpdateBaseMtx(u32 msg) {
    return msg == ACTMES_UPDATE_BASEMTX;
}

bool MR::isMsgRushCancel(u32 msg) {
    return msg == ACTMES_RUSH_CANCEL;
}

bool MR::isMsgIsRushTakeOver(u32 msg) {
    return msg == ACTMES_IS_RUSH_TAKEOVER;
}

bool MR::isMsgFloorTouch(u32 msg) {
    return msg == ACTMES_FLOOR_TOUCH;
}

bool MR::isMsgWallTouch(u32 msg) {
    return msg == ACTMES_WALL_TOUCH;
}

bool MR::isMsgCeilTouch(u32 msg) {
    return msg == ACTMES_CEIL_TOUCH;
}

bool MR::isMsgItemGet(u32 msg) {
    return msg == ACTMES_ITEM_GET;
}

bool MR::isMsgItemPull(u32 msg) {
    return msg == ACTMES_ITEM_PULL;
}

bool MR::isMsgItemShow(u32 msg) {
    return msg == ACTMES_ITEM_SHOW;
}

bool MR::isMsgItemHide(u32 msg) {
    return msg == ACTMES_ITEM_HIDE;
}

bool MR::isMsgItemStartMove(u32 msg) {
    return msg == ACTMES_ITEM_START_MOVE;
}

bool MR::isMsgItemEndMove(u32 msg) {
    return msg == ACTMES_ITEM_END_MOVE;
}

bool MR::isMsgInhaleBlackHole(u32 msg) {
    return msg == ACTMES_INHALE_BLACK_HOLE;
}

bool MR::isMsgEnemyAttack(u32 msg) {
    return msg == ACTMES_ENEMY_ATTACK;
}

bool MR::isMsgEnemyAttackFire(u32 msg) {
    return msg == ACTMES_ENEMY_ATTACK_FIRERUN;
}

bool MR::isMsgEnemyAttackFireStrong(u32 msg) {
    return msg == ACTMES_ENEMY_ATTACK_FIRERUN_STRONG;
}

bool MR::isMsgEnemyAttackElectric(u32 msg) {
    return msg == ACTMES_ENEMY_ATTACK_ELECTRIC;
}

bool MR::isMsgExplosionAttack(u32 msg) {
    return msg == ACTMES_ENEMY_ATTACK_EXPLOSION;
}

bool MR::isMsgToEnemyAttackBlow(u32 msg) {
    return msg == ACTMES_TO_ENEMY_ATTACK_BLOW;
}

bool MR::isMsgToEnemyAttackTrample(u32 msg) {
    return msg == ACTMES_TO_ENEMY_ATTACK_TRAMPLE;
}

bool MR::isMsgToEnemyAttackShockWave(u32 msg) {
    return msg == ACTMES_TO_ENEMY_ATTACK_SHOCK_WAVE;
}

bool MR::isMsgSpinStormRange(u32 msg) {
    return msg == ACTMES_SPIN_STORM_RANGE;
}

bool MR::isMsgTutorialStart(u32 msg) {
    return msg == ACTMES_TUTORIAL_START;
}

bool MR::isMsgTutorialNext(u32 msg) {
    return msg == ACTMES_TUTORIAL_NEXT;
}

bool MR::isMsgTutorialPrev(u32 msg) {
    return msg == ACTMES_TUTORIAL_PREV;
}

bool MR::isMsgTutorialPass(u32 msg) {
    return msg == ACTMES_TUTORIAL_PASS;
}

bool MR::isMsgTutorialOmit(u32 msg) {
    return msg == ACTMES_TUTORIAL_OMIT;
}

bool MR::isMsgRaceReady(u32 msg) {
    return msg == ACTMES_RACE_READY;
}

bool MR::isMsgRaceStart(u32 msg) {
    return msg == ACTMES_RACE_START;
}

bool MR::isMsgRaceReset(u32 msg) {
    return msg == ACTMES_RACE_RESET;
}

bool MR::isMsgLockOnStarPieceShoot(u32 msg) {
    return msg == ACTMES_IS_LOCKON_STAR_PIECE_SHOOT;
}

bool MR::isMsgBallDashWall(u32 msg) {
    return msg == ACTMES_BALL_DASH_WALL;
}

bool MR::isMsgBallDashGround(u32 msg) {
    return msg == ACTMES_BALL_DASH_GROUND;
}

bool MR::isMsgStartPowerStarGet(u32 msg) {
    return msg == ACTMES_START_POWER_STAR_GET;
}

bool MR::isMsgTouchPlantItem(u32 msg) {
    return msg == ACTMES_PLANT_GROUP_EMIT_ITEM;
}

bool MR::isMsgHitmarkEmit(u32 msg) {
    return msg == ACTMES_HITMARK_EMIT;
}

bool MR::isMsgStarPieceAttack(u32 msg) {
    return msg == ACTMES_STAR_PIECE_ATTACK;
}

bool MR::isMsgStarPieceReflect(u32 msg) {
    return msg == ACTMES_IS_STAR_PIECE_REFLECT;
}

bool MR::isMsgStarPieceGift(u32 msg) {
    return msg >= ACTMES_STAR_PIECE_GIFT && msg <= ACTMES_STAR_PIECE_GIFT_MAX;
}

s32 MR::getNumStarPieceGift(u32 msg) {
    return (msg - ACTMES_STAR_PIECE_GIFT) + 1;
}

void MR::calcPosBetweenSensors(TVec3f* pPos, const HitSensor* pSensor1, const HitSensor* pSensor2, f32 offset) {
    TVec3f dir = pSensor2->mPosition - pSensor1->mPosition;
    normalizeOrZero(&dir);

    f32 dist = pSensor1->mPosition.distance(pSensor2->mPosition);
    f32 radius2 = pSensor2->mRadius;
    f32 radius1 = pSensor1->mRadius;
    f32 value = (radius1 + radius2 - dist) / 2.0f;

    pPos->set(dir);
    *pPos *= value + pSensor1->mRadius + offset;
    pPos->add(pSensor1->mPosition);
}

bool MR::tryForceKillIfMsgStartPowerStarGet(LiveActor* pActor, u32 msg) {
    if (msg == ACTMES_START_POWER_STAR_GET) {
        if (isExistEffectKeeper(pActor)) {
            forceDeleteEffectAll(pActor);
        }

        pActor->makeActorDead();

        return true;
    }

    return false;
}

HitSensor* MR::addBodyMessageSensor(LiveActor* pActor, u32 type) {
    return addHitSensor(pActor, "body", type, 0, 0.0f, TVec3f(0.0f, 0.0f, 0.0f));
}

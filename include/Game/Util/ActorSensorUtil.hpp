#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include <revolution.h>

class LiveActor;
class HitSensor;

namespace MR {
    HitSensor* addHitSensor(LiveActor *, const char *, u32, u16, f32, const TVec3f &);
    HitSensor* addHitSensorBinder(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorTransferableBinder(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorPriorBinder(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorRide(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorMapObj(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorMapObjPress(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorMapObjSimple(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorMapObjMoveCollision(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorEnemy(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorEnemySimple(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorEnemyAttack(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorNpc(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorEye(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorPush(LiveActor *, const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorPosBinder(LiveActor *, const char *, u16, f32, const TVec3f *, const TVec3f &);
    HitSensor* addHitSensorPosRide(LiveActor *, const char *, u16, f32, const TVec3f *, const TVec3f &);
    HitSensor* addHitSensorPosMapObj(LiveActor *, const char *, u16, f32, const TVec3f *, const TVec3f &);
    HitSensor* addHitSensorPosEye(LiveActor *, const char *, u16, f32, const TVec3f *, const TVec3f &);
    HitSensor* addHitSensorMtx(LiveActor *, const char *, u32, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxRide(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxMapObj(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxEnemy(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxEnemyAttack(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxNpc(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorMtxAnimal(LiveActor *, const char *, u16, f32, MtxPtr, const TVec3f &);
    HitSensor* addHitSensorAtJoint(LiveActor *, const char *,  const char *, u32, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointRide(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointMapObj(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointMapObjSimple(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointEnemy(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointEnemySimple(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointEnemyAttack(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointNpc(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorAtJointEye(LiveActor *, const char *,  const char *, u16, f32, const TVec3f &);
    HitSensor* addHitSensorCallback(LiveActor *, const char *, u32, u16, f32);
    HitSensor* addHitSensorCallbackBinder(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackPriorBinder(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackRide(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackMapObj(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackMapObjSimple(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackEnemy(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackEnemyAttack(LiveActor *, const char *, u16, f32);
    HitSensor* addHitSensorCallbackEye(LiveActor *, const char *, u16, f32);

    HitSensor* addBodyMessageSensorMapObj(LiveActor *);
    void addBodyMessageSensor(LiveActor *, u32);
    HitSensor* addBodyMessageSensorMapObjMoveCollision(LiveActor *);
    HitSensor* addBodyMessageSensorReceiver(LiveActor *);

    HitSensor* addMessageSensorMapObjMoveCollision(LiveActor *, const char *);

    LiveActor* getSensorHost(const HitSensor *);
    bool isSensor(const HitSensor *, const char *);
    bool isSensorPlayer(const HitSensor *);
    bool isSensorBinder(const HitSensor *);
    bool isSensorRide(const HitSensor *);

    bool isSensorPlayerOrRide(const HitSensor *);

    bool isSensorEnemyAttack(const HitSensor *);

    bool isMsgLockOnStarPieceShoot(u32);

    bool isMsgHitmarkEmit(u32);
    bool isMsgStarPieceAttack(u32);
    bool isMsgStarPieceReflect(u32);

    bool isMsgEnemyAttackFire(u32);
    bool isMsgEnemyAttackFireStrong(u32);

    bool isMsgToEnemyAttackBlow(u32);
    bool isMsgToEnemyAttackTrample(u32);

	bool isMsgRushCancel(u32);
    bool isMsgIsRushTakeOver(u32);
    bool isMsgFloorTouch(u32);

    bool isSensorEnemy(const HitSensor *);

    bool isSensorMapObj(const HitSensor *);
    bool isSensorAutoRush(const HitSensor *);
    bool isSensorRush(const HitSensor *);
    bool isSensorPressObj(const HitSensor *);
    bool isSensorEye(const HitSensor *);
    bool isSensorPush(const HitSensor *);
    bool isSensorItem(const HitSensor *);
    bool tryGetItem(HitSensor *, HitSensor *);
    TVec3f* getSensorPos(const HitSensor *);

    bool isValidHitSensor(LiveActor *, const char *);

    void validateHitSensors(LiveActor *);
    void invalidateHitSensors(LiveActor *);

    void calcSensorDirection(TVec3f *, const HitSensor *, const HitSensor *);

    void calcSensorDirectionNormalize(TVec3f *, const HitSensor *, const HitSensor *);
    void calcSensorHorizonNormalize(TVec3f *, const TVec3f &, const HitSensor *, const HitSensor *);

    bool sendArbitraryMsg(u32, HitSensor *, HitSensor *);
    bool sendMsgPush(HitSensor *, HitSensor *);
    bool sendMsgPlayerTrample(HitSensor *, HitSensor *);
    bool sendMsgPlayerPunch(HitSensor *, HitSensor *);
    bool sendMsgJump(HitSensor *, HitSensor *);
    bool sendMsgTouchJump(HitSensor *, HitSensor *);
    bool sendMsgTaken(HitSensor *, HitSensor *);
    bool sendMsgKick(HitSensor *, HitSensor *);
    bool sendMsgAwayJump(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackMsgToDir(u32, HitSensor *, HitSensor *, const TVec3f &);
    bool sendMsgEnemyAttackFlipWeak(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipWeakJump(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlip(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipToDir(HitSensor *, HitSensor *, const TVec3f &);
    bool sendMsgEnemyAttackFlipJump(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipRot(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipMaximum(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor *, HitSensor *, const TVec3f &);
    bool sendMsgEnemyAttack(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackStrong(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackStringToDir(HitSensor *, HitSensor *, const TVec3f &);
    bool sendMsgEnemyAttackFire(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFireStrong(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackElectric(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFreeze(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackHeatBeam(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackExplosion(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackCounterSpin(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackCounterHipDrop(HitSensor *, HitSensor *);
    bool sendMsgLockOnStarPieceShoot(HitSensor *, HitSensor *);
    bool sendMsgStarPieceAttack(HitSensor *, HitSensor *);
    bool sendMsgStarPieceGift(HitSensor *, HitSensor *, u32);
    bool sendMsgEnemyAttackMaximum(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackMaximumToDir(HitSensor *, HitSensor *, const TVec3f &);

    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor *, HitSensor *);

    bool sendMsgEnemyAttackToBindedSensor(LiveActor *, HitSensor *);

    bool sendMsgToEnemyAttackBlow(HitSensor *, HitSensor *);

    void sendMsgToGroupMember(u32, LiveActor *, HitSensor *, const char *);

    bool isMsgPlayerHitAll(u32);
    bool isMsgPlayerSpinAttack(u32);
    bool isMsgPlayerTrample(u32);
    bool isMsgPlayerHipDrop(u32);
    bool isMsgPlayerHipDropFloor(u32);
    bool isMsgPlayerUpperPunch(u32);
    bool isMsgPlayerKick(u32);
    bool isMsgJetTurtleAttack(u32);

    bool isMsgItemGet(u32);
    bool isMsgItemPull(u32);
    bool isMsgItemShow(u32);
    bool isMsgItemHide(u32);
    bool isMsgItemStartMove(u32);
    bool isMsgItemEndMove(u32);
    bool isMsgInhaleBlackHole(u32);

    bool isMsgFreezeAttack(u32);
    bool isMsgInvincibleAttack(u32);

    bool isMsgFireBallAttack(u32);

    bool isMsgSpinStormRange(u32);

    bool isMsgAutoRushBegin(u32);

    bool isMsgExplosionAttack(u32);

    bool isMsgUpdateBaseMtx(u32);

    bool receiveItemShowMsg(u32, HitSensor *, HitSensor *);
    bool receiveItemHideMsg(u32, HitSensor *, HitSensor *);

    void updateHitSensorsAll(LiveActor *);

    void setHitSensorApart(HitSensor *, HitSensor *);

    bool tryUpdateHitSensorsAll(LiveActor *);

    void clearHitSensors(LiveActor *);

    HitSensor* getGroundSensor(const LiveActor *);
    HitSensor* getRoofSensor(const LiveActor *);
    HitSensor* getWallSensor(const LiveActor *);

    void setSensorRadius(LiveActor *, const char *, f32);

    void setSensorOffset(LiveActor *, const char *, const TVec3f &);

    void validateHitSensor(LiveActor *, const char *);
    void invalidateHitSensor(LiveActor *, const char *);

    bool isInSpinStormRange(u32, HitSensor *, HitSensor *, f32);
};

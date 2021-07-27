#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec3.h"

class LiveActor;
class HitSensor;

namespace MR
{
    void addHitSensor(LiveActor *, const char *, u32, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorBinder(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorTransferableBinder(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorPriorBinder(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorRide(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorMapObjPress(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorMapObjSimple(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorMapObjMoveCollision(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorEnemy(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorEnemySimple(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorEnemyAttack(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorNpc(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorEye(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorPush(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    
    void addHitSensorPosBinder(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);
    void addHitSensorPosRide(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);
    void addHitSensorPosMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);
    void addHitSensorPosEye(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);
    
    void addHitSensorMtx(LiveActor *, const char *, u32, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxRide(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxMapObj(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxEnemy(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxEnemyAttack(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxNpc(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    void addHitSensorMtxAnimal(LiveActor *, const char *, u16, Mtx *, const JGeometry::TVec3<f32> &);
    
    void addHitSensorAtJoint(LiveActor *, const char *, const char *, u32, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointRide(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointMapObj(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointMapObjSimple(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointEnemy(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointEnemySimple(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointEnemyAttack(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointNpc(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    void addHitSensorAtJointEye(LiveActor *, const char *, const char *, u16, f32, const JGeometry::TVec3<f32> &);
    
    void addHitSensorCallback(LiveActor *, const char *, const char *, u32, u16, f32);
    void addHitSensorCallbackBinder(LiveActor *, const char *, const char *, u16, f32);
    void addHitSensorCallbackPriorBinder(LiveActor *, const char *, const char *, u16, f32);
    void addHitSensorCallbackRide(LiveActor *, const char *, u16, f32);
    void addHitSensorCallbackMapObj(LiveActor *, const char *, u16, f32);
    void addHitSensorCallbackMapObjSimple(LiveActor *, const char *, u16, f32);
    void addHitSensorCallbackEnemy(LiveActor *, const char *, u16, f32);
    void addHitSensorCallbackEnemyAttack(LiveActor *, const char *, u16, f32);
    void addHitSensorCallbackEye(LiveActor *, const char *, u16, f32);
    
    void addBodyMessageSensorReciever(LiveActor *);
    void addBodyMessageSensorMapObj(LiveActor *);
    void addBodyMessageSensorMapObjPress(LiveActor *);
    void addBodyMessageSensorMapObjMoveCollision(LiveActor *);
    void addBodyMessageSensorEnemy(LiveActor *);

    void addMessageSensorReciever(LiveActor *, const char *);
    void addMessageSensorMapObj(LiveActor *, const char *);
    void addMessageSensorMapObjMoveCollision(LiveActor *, const char *);
    void addMessageSensorEnemy(LiveActor *, const char *);

    bool tryUpdateHitSensorsAll(LiveActor *);
    void updateHitSensorsAll(LiveActor *);
    bool isSensorType(const HitSensor *, u32);
    HitSensor* getSensorWithIndex(LiveActor *, s32);
    HitSensor* getTaking(const LiveActor *);
    HitSensor* getTaken(const LiveActor *);

    void setSensorPos(HitSensor *, const JGeometry::TVec3<f32> &);
    void setSensorOffset(LiveActor *, const char *, JGeometry::TVec3<f32> &);
    void setSensorRadius(LiveActor *, const char *, f32);
    void setHitSensorApart(HitSensor *, HitSensor *);

    void validateHitSensors(LiveActor *);
    void invalidateHitSensors(LiveActor *);
    void validateHitSensor(LiveActor *, const char *);
    void invalidateHitSensor(LiveActor *, const char *);
    bool isValidHitSensor(LiveActor *, const char *);
    void clearHitSensors(LiveActor *);
    HitSensor* getSensor(LiveActor *, s32);
    LiveActor* getSensorHost(const HitSensor *);

    bool isSensor(const HitSensor *, const char *);
    bool isSensorPlayer(const HitSensor *);
    bool isSensorBinder(const HitSensor *);
    bool isSensorRide(const HitSensor *);
    bool isSensorPlayerOrRide(const HitSensor *);
    bool isSensorEnemy(const HitSensor *);
    bool isSensorEnemyAttack(const HitSensor *);
    bool isSensorNpc(const HitSensor *);
    bool isSensorMapObj(const HitSensor *);
    bool isSensorAutoRush(const HitSensor *);
    bool isSensorRush(const HitSensor *);
    bool isSensorPressObj(const HitSensor *);
    bool isSensorEye(const HitSensor *);
    bool isSensorPush(const HitSensor *);
    bool isSensorItem(const HitSensor *);
    bool tryGetItem(HitSensor *, HitSensor *);

    JGeometry::TVec3<f32> getSensorPos(const HitSensor *);
    void calcSensorDirection(JGeometry::TVec3<f32> *, const HitSensor *, const HitSensor *);
    void calcSensorDirectionNormalize(JGeometry::TVec3<f32> *, const HitSensor *, const HitSensor *);
    void calcSensorHorizonNormalize(JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &, const HitSensor *, const HitSensor *);

    HitSensor* getMessageSensor();

    bool sendArbitraryMsg(u32, HitSensor *, HitSensor *);
    bool sendMsgPush(HitSensor *, HitSensor *);
    bool sendMsgPlayerTrample(HitSensor *, HitSensor *);
    bool sendMsgPlayerPunch(HitSensor *, HitSensor *);
    bool sendMsgJump(HitSensor *, HitSensor *);
    bool sendMsgTouchJump(HitSensor *, HitSensor *);
    bool sendMsgTaken(HitSensor *, HitSensor *);
    bool sendMsgKick(HitSensor *, HitSensor *);
    bool sendMsgAwayJump(HitSensor *, HitSensor *);

    bool sendMsgEnemyAttackMsgToDir(u32, HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    bool sendMsgEnemyAttackFlipWeak(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipWeakJump(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlip(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    bool sendMsgEnemyAttackFlipJump(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipRot(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipMaximum(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackFlipMaximumToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    bool sendMsgEnemyAttack(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackStrong(HitSensor *, HitSensor *);
    bool sendMsgEnemyAttackStrongToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
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
    bool sendMsgEnemyAttackMaximumToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    bool sendMsgEnemyAttackToBindedSensor(LiveActor *, HitSensor *);
    bool sendMsgEnemyAttackExplosionToBindedSensor(LiveActor *, HitSensor *);

    bool sendSimpleMsgToActor(u32, LiveActor *);
    bool sendMsgToBindedSensor(u32, LiveActor *, HitSensor *);
    bool sendMsgToBindedSensor(u32, HitSensor *);
    bool sendMsgToGroundSensor(u32, HitSensor *);
    bool sendMsgToWallSensor(u32, HitSensor *);

    bool sendMsgStartDemo(LiveActor *);
    bool sendMsgToEnemyAttackShockWave(HitSensor *, HitSensor *);

    bool sendMsgToAllLiveActor(u32, LiveActor *);
    bool sendMsgToGroupMember(u32, LiveActor *, HitSensor *, const char *);
    bool sendMsgExplosionToNearActor(HitSensor *, f32);
    
    bool isInSpinStormRange(u32, HitSensor *, HitSensor *, f32);
    bool recieveItemShowMsg(u32, HitSensor *, HitSensor *);
    bool recieveItemHideMsg(u32, HitSensor *, HitSensor *);

    HitSensor* getGroundSensor(const LiveActor *);
    HitSensor* getRoofSensor(const LiveActor *);
    HitSensor* getWallSensor(const LiveActor *);

    bool isMsgPlayerHitAll(u32);
    bool isMsgPlayerSpinAttack(u32);
    bool isMsgPlayerTrample(u32);
    bool isMsgPlayerHitDrop(u32);
    bool isMsgPlayerHipDropFloor(u32);
    bool isMsgPlayerUpperPunch(u32);
    bool isMsgPlayerKick(u32);
    bool isMsgJetTurtleAttack(u32);
    bool isMsgFireBallAttack(u32);
    bool isMsgSearchlightAttack(u32);
    bool isMsgFreezeAttack(u32);
    bool isMsgInvincibleAttack(u32);
    bool isMsgInvalidHit(u32);
    bool isMsgAutoRushBegin(u32);
    bool isMsgRushBegin(u32);
    bool isMsgUpdateBaseMtx(u32);
    bool isMsgRushCancel(u32);
    bool isMsgIsRushTakeOver(u32);
    bool isMsgFloorTouch(u32);
    bool isMsgWallTouch(u32);
    bool isMsgCeilTouch(u32);
    bool isMsgItemGet(u32);
    bool isMsgItemPull(u32);
    bool isMsgItemShow(u32);
    bool isMsgItemHide(u32);
    bool isMsgItemStartMove(u32);
    bool isMsgItemEndMove(u32);
    bool isMsgInhaleBlackHole(u32);
    bool isMsgEnemyAttack(u32);
    bool isMsgEnemyAttackFire(u32);
    bool isMsgEnemyAttackFireStrong(u32);
    bool isMsgEnemyAttackElectric(u32);
    bool isMsgExplosionAttack(u32);
    bool isMsgToEnemyAttackBlow(u32);
    bool isMsgToEnemyAttackTrample(u32);
    bool isMsgToEnemyAttackShowWave(u32);
    bool isMsgSpinStormRange(u32);
    bool isMsgTutorialStart(u32);
    bool isMsgTutorialNext(u32);
    bool isMsgTutorialPrev(u32);
    bool isMsgTutorialPass(u32);
    bool isMsgTutorialOmit(u32);
    bool isMsgRaceReady(u32);
    bool isMsgRaceStart(u32);
    bool isMsgRaceReset(u32);
    bool isMsgLockOnStarPieceShoot(u32);
    bool isMsgBallDashWall(u32);
    bool isMsgBallDashGround(u32);
    bool isMsgStartPowerStarGet(u32);
    bool isMsgTouchPlantItem(u32);
    bool isMsgHitmarkEmit(u32);
    bool isMsgStarPieceAttack(u32);
    bool isMsgStarPieceReflect(u32);
    bool isMsgStarPieceGift(u32);
    u32 getNumStarPieceGift(u32);

    void calcPosBetweenSensors(JGeometry::TVec3<f32> *, const HitSensor *, const HitSensor *, f32);
    bool tryForceKillIfMsgStarPowerStarGet(LiveActor *, u32);
    void addBodyMessageSensor(LiveActor *, u32);
};
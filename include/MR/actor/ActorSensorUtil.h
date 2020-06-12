#ifndef ACTORSENSORUTIL_H
#define ACTORSENSORUTIL_H

#include <revolution.h>
#include "JGeometry/TVec3.h"

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

    void sendArbitraryMsg(u32, HitSensor *, HitSensor *);
    void sendMsgPush(HitSensor *, HitSensor *);
    void sendMsgPlayerTrample(HitSensor *, HitSensor *);
    void sendMsgPlayerPunch(HitSensor *, HitSensor *);
    void sendMsgJump(HitSensor *, HitSensor *);
    void sendMsgTouchJump(HitSensor *, HitSensor *);
    void sendMsgTaken(HitSensor *, HitSensor *);
    void sendMsgKick(HitSensor *, HitSensor *);
    void sendMsgAwayJump(HitSensor *, HitSensor *);

    void sendMsgEnemyAttackMsgToDir(u32, HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    void sendMsgEnemyAttackFlipWeak(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlipWeakJump(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlip(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlipToDir(HitSensor *, HitSensor *, JGeometry::TVec3<f32> &);
    void sendMsgEnemyAttackFlipJump(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlipRot(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlipMaximum(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFlipMaximumToDir(HitSensor *, HitSensor *, JGeometry::TVec3<f32> &);
    void sendMsgEnemyAttack(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackStrong(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackStrongToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    void sendMsgEnemyAttackFire(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFireStrong(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackElectric(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackFreeze(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackHeatBeam(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackExplosion(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackCounterSpin(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackCounterHipDrop(HitSensor *, HitSensor *);

    void sendMsgLockOnStarPieceShoot(HitSensor *, HitSensor *);
    void sendMsgStarPieceAttack(HitSensor *, HitSensor *);
    void sendMsgStarPieceGift(HitSensor *, HitSensor *);

    void sendMsgEnemyAttackMaximum(HitSensor *, HitSensor *);
    void sendMsgEnemyAttackMaximumToDir(HitSensor *, HitSensor *, const JGeometry::TVec3<f32> &);
    void sendMsgEnemyAttackToBindedSensor(LiveActor *, HitSensor *);
    void sendMsgEnemyAttackExplosionToBindedSensor(LiveActor *, HitSensor *);

    void sendSimpleMsgToActor(u32, LiveActor *);
    void sendMsgToBindedSensor(u32, LiveActor *, HitSensor *);
    void sendMsgToBindedSensor(u32, HitSensor *);
    void sendMsgToGroundSensor(u32, HitSensor *);
    void sendMsgToWallSensor(u32, HitSensor *);

    void sendMsgStartDemo(LiveActor *);
    void sendMsgToEnemyAttackShockWave(HitSensor *, HitSensor *);

    void sendMsgToAllLiveActor(u32, LiveActor *);
    void sendMsgToGroupMember(u32, LiveActor *, HitSensor *, const char *);
    void sendMsgExplosionToNearActor(HitSensor *, f32);
    
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

#endif // ACTORSENSORUTIL_H
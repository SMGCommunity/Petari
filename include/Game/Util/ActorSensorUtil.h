#pragma once

#include "JSystem/JGeometry/TVec.h"
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

    bool isMsgPlayerHitAll(u32);
    bool isMsgPlayerSpinAttack(u32);
    bool isMsgPlayerTrample(u32);
    bool isMsgPlayerHipDrop(u32);
    bool isMsgPlayerHipDropFloor(u32);
    bool isMsgPlayerUpperPunch(u32);
    bool isMsgPlayerKick(u32);
    bool isMsgJetTurtleAttack(u32);
};

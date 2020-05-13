#ifndef ACTORSENSORUTIL_H
#define ACTORSENSORUTIL_H

#include <revolution.h>
#include "JGeometry/TVec3.h"

class LiveActor;
class HitSensor;

namespace MR
{
    u8 tryUpdateHitSensorsAll(LiveActor *);
    void updateHitSensorsAll(LiveActor *);
    void clearHitSensors(LiveActor *);
    HitSensor* getTaken(const LiveActor *);
    HitSensor* getTaking(const LiveActor *);

    void validateHitSensors(LiveActor *);
    void invalidateHitSensors(LiveActor *);
    u8 isSensor(const HitSensor *, const char *);
    u8 isSensorPlayer(const HitSensor *);

    u32 setHitSensorApart(HitSensor *, HitSensor *);

    void sendMsgPush(HitSensor *, HitSensor *);

    void sendMsgEnemyAttackElectric(HitSensor *, HitSensor *);

    void addHitSensorMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);

    void addHitSensorPosMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);

    u8 isMsgPlayerHitAll(u32);
    bool isMsgPlayerSpinAttack(u32);
};

#endif // ACTORSENSORUTIL_H
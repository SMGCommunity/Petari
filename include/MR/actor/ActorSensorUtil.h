#ifndef ACTORSENSORUTIL_H
#define ACTORSENSORUTIL_H

class LiveActor;

namespace MR
{
    bool tryUpdateHitSensorsAll(LiveActor *);
    void updateHitSensorsAll(LiveActor *);
    void clearHitSensors(LiveActor *);
    HitSensor* getTaken(const LiveActor *);
    HitSensor* getTaking(const LiveActor *);

    void validateHitSensors(LiveActor *);
    void invalidateHitSensors(LiveActor *);
    bool isSensor(const HitSensor *, const char *);
    bool isSensorPlayer(const HitSensor *);

    u32 setHitSensorApart(HitSensor *, HitSensor *);

    void sendMsgPush(HitSensor *, HitSensor *);

    void sendMsgEnemyAttackElectric(HitSensor *, HitSensor *);

    void addHitSensorMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> &);

    void addHitSensorPosMapObj(LiveActor *, const char *, u16, f32, const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> &);
};

#endif // ACTORSENSORUTIL_H
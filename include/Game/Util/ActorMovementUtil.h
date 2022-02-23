#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"
#include "JSystem/JGeometry/TMatrix.h"

class LiveActor;
class HitSensor;

namespace MR {
    f32 calcDistance(const HitSensor *, const HitSensor *, TVec3f *);
    f32 calcDistance(const LiveActor *, const TVec3f &);
    f32 calcDistanceHorizontal(const LiveActor *, const TVec3f &, const TVec3f &);
    f32 calcDistanceVertical(const LiveActor *, const TVec3f &, const TVec3f &);
    f32 calcDistanceVertical(const LiveActor *, const TVec3f &);
    f32 calcDistanceToPlayer(const LiveActor *);
    f32 calcDistanceToPlayerH(const LiveActor *);
    bool isNear(const HitSensor *, const HitSensor *, f32);
    bool isNear(const LiveActor *, const LiveActor *, f32);
    bool isNear(const LiveActor *, const TVec3f &, f32);
    bool isNearPlayer(const LiveActor *, f32);
    bool isNearPlayerAnyTime(const LiveActor *, f32);
    bool isNearPlayer(const TVec3f &, f32);
    bool isNearPlayerPose(const LiveActor *, f32, f32);
    bool isNearPlayerHorizontal(const LiveActor *, f32);
    void calcFrontVec(TVec3f *, const LiveActor *);
    void calcUpVec(TVec3f *, const LiveActor *);
    void calcSideVec(TVec3f *, const LiveActor *);
    void calcPositionUpOffset(TVec3f *, const LiveActor *, f32);

    void resetPosition(LiveActor *);

    void calcActorAxis(TVec3f *, TVec3f *, TVec3f *, const LiveActor *);

    void calcActorAxisY(TVec3f *, const LiveActor *);

    bool isInWater(const LiveActor *, const TVec3f &);

    void attenuateVelocity(LiveActor *, f32);

    void killVelocityToTarget(LiveActor *, const TVec3f &);

    void restrictVelocity(LiveActor *, f32);

    void turnDirectionToGround(const LiveActor *, TVec3f *);

    void calcMtxFromGravityAndZAxis(TPos3f *, const LiveActor *, const TVec3f &, const TVec3f &);

    void addVelocityToGravity(LiveActor *, f32);

    void resetPosition(LiveActor *, const TVec3f &);
};

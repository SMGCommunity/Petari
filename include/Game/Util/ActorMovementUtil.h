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
    void calcVecToPlayerH(TVec3f *, const LiveActor *, const TVec3f *);
    void calcVecFromPlayerH(TVec3f *, const LiveActor *);
    void calcVecToTargetPosH(TVec3f *, const LiveActor *, const TVec3f &, const TVec3f *);
    void calcVecFromTargetPosH(TVec3f *, const LiveActor *, const TVec3f &, const TVec3f *);
    bool isFaceToTargetDegree(const LiveActor *, const TVec3f &, const TVec3f &, float);
    bool isFaceToPlayerDegree(const LiveActor *, const TVec3f &, float);
    bool isFaceToTargetDegree(const LiveActor *, const TVec3f &, float);
    bool isFaceToPlayerDegree(const LiveActor *, float);
    bool isFaceToPlayerDegreeHV(const LiveActor *, const TVec3f&, float, float);
    bool isFaceToTargetHorizontalDegree(const LiveActor *, const TVec3f &, const TVec3f &, float);
    bool isFaceToPlayerHorizontalDegree(const LiveActor *, const TVec3f &, float);
    bool isClockwiseToDir(const LiveActor *, const TVec3f &, const TVec3f &);
    bool isClockwiseToPlayer(const LiveActor *, const TVec3f &);
    bool isPlayerLeftSide(const LiveActor *);
    bool isLeftSideFromPlayer(const LiveActor *);
    bool isInSightConePlayer(const LiveActor *, const TVec3f &, float, float);
    bool isInSightFanPlayer(const LiveActor *, const TVec3f &, float, float, float);
    bool isBindedWallFront(const LiveActor *, const TVec3f &, float);
    bool isPlayerExistSide(const LiveActor *, float, float);
    bool isPlayerExistUp(const LiveActor *, float, float);
    bool isPlayerExistDown(const LiveActor *, float, float);
    bool isInWater(const LiveActor *, const TVec3f &);
    bool isInDeath(const LiveActor *, const TVec3f &);

    void makeMtxTR(MtxPtr, const LiveActor *);
    void makeMtxTRS(MtxPtr, const LiveActor *);
    void makeMtxTransRotateY(MtxPtr, const LiveActor *);

    void blendQuatFromGroundAndFront(TQuat4f *, const LiveActor *, const TVec3f &, float, float);

    void resetPosition(LiveActor *);
    void resetPosition(LiveActor *, const TVec3f &);
    void resetPosition(LiveActor *, const char *);

    void calcActorAxis(TVec3f *, TVec3f *, TVec3f *, const LiveActor *);

    void calcActorAxisY(TVec3f *, const LiveActor *);

    void attenuateVelocity(LiveActor *, f32);

    void killVelocityToTarget(LiveActor *, const TVec3f &);

    void restrictVelocity(LiveActor *, f32);

    void turnDirectionToGround(const LiveActor *, TVec3f *);

    void calcMtxFromGravityAndZAxis(TPos3f *, const LiveActor *, const TVec3f &, const TVec3f &);

    void addVelocityToGravity(LiveActor *, f32);

    void addVelocityKeepHeight(LiveActor *, const TVec3f &, f32, f32, f32);

    void reboundVelocityFromCollision(LiveActor *, f32, f32, f32);

    void addVelocity(LiveActor *, const TVec3f &);

    f32 calcVelocityLength(const LiveActor *);

    void turnDirectionAndGravityH(LiveActor *, const TVec3f &, f32, f32);

    void turnDirectionToTargetUseGroundNormalDegree(const LiveActor *, TVec3f *, const TVec3f &, f32);

    void turnDirectionToTargetDegree(const LiveActor *, TVec3f *, const TVec3f &, f32);
};

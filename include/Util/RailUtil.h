#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include "JSystem/JGeometry/TBox3.h"
#include <revolution.h>

class LiveActor;
class RailRider;

namespace
{
    void updateBoundingBox(const RailRider *, JGeometry::TBox3<f32> *);
}

namespace MR
{
    void moveTransToOtherActorRailPos(LiveActor *, const LiveActor *);
    void moveTransToCurrentRailPos(LiveActor *);
    void moveCoord(LiveActor *, f32);
    void moveCoordAndTransToRailPoint(LiveActor *, s32);
    s32 moveCoordAndCheckPassPointNo(LiveActor *, f32);
    s32 moveCoordAndFollowTransAndCheckPassPointNo(LiveActor *);
    void moveCoordToRailPoint(LiveActor *, s32);
    void moveCoordToNearestPos(LiveActor *, const JGeometry::TVec3f &);
    void moveCoordAndFollowTrans(LiveActor *, f32);
    void moveCoordAndFollowTrans(LiveActor *);
    void moveCoordToStartPos(LiveActor *);
    void moveCoordToEndPos(LiveActor *);
    void moveCoordAndTransToNearestRailPos(LiveActor *);
    void moveCoordTransToNearestRailPos(LiveActor *, const JGeometry::TVec3f &);
    void moveCoordAndTransToNearestRailPoint(LiveActor *);
    void moveCoordAndTransToNextPoint(LiveActor *);
    void moveCoordAndTransToRailStartPoint(LiveActor *);
    
    void setRailDirectionToStart(LiveActor *);
    void setRailDirectionToEnd(LiveActor *);
    void setRailDirectionCloseToCoord(LiveActor *, f32);
    void setRailDirectionCloseToNearestPos(LiveActor *, const JGeometry::TVec3f &);

    void reverseRailDirection(LiveActor *);
    f32 calcMovingDirectionAlongRail(LiveActor *, JGeometry::TVec3f *, const JGeometry::TVec3f &, f32, bool, bool *);
    f32 calcMovingDirectionAlongRailH(LiveActor *, JGeometry::TVec3f *, const JGeometry::TVec3f &, f32, bool, bool *);
    void calcRailClippingInfo(JGeometry::TVec3f *, f32 *, const LiveActor *, f32, f32);
    void initAndSetRailClipping(JGeometry::TVec3f *, LiveActor *, f32, f32);

    f32 calcNearestRailCoord(const LiveActor *, const JGeometry::TVec3f &);
    f32 calcNearestRailPos(JGeometry::TVec3f *, const LiveActor *, const JGeometry::TVec3f &);
    f32 calcNearestRailDirection(JGeometry::TVec3f *, const LiveActor *, const JGeometry::TVec3f &);
    f32 calcNearestRailPosAndDirection(JGeometry::TVec3f *, JGeometry::TVec3f *, const LiveActor *, const JGeometry::TVec3f &);
    f32 calcRailPosNearestPlayer(JGeometry::TVec3f *, const LiveActor *);

    void calcBoundingBox(RailRider *, JGeometry::TBox3<f32> *, f32);
    void calcBoundingBox(const LiveActor *, JGeometry::TBox3<f32> *, f32);
    void calcDifferenceRailCoord(const LiveActor *, f32, f32);
    void calcDifferenceRailCoord(const LiveActor *, f32);
    void calcDistanceHorizonToCurrentPos(const LiveActor *);
    void calcRailPosAtCoord(JGeometry::TVec3f *, const LiveActor *, f32);

    bool isExistRail(const LiveActor *);
    s32 getRailPointNum(const LiveActor *);
    s32 getCurrentRailPointNo(const LiveActor *);

    f32 getRailTotalLength(const LiveActor *);
    JGeometry::TVec3f getRailDirection(const LiveActor *);
    JGeometry::TVec3f getRailCoord(const LiveActor *);
    f32 getRailCoordSpeed(const LiveActor *);
    void setRailCoord(LiveActor *, f32);
    void setRailCoordSpeed(LiveActor *, f32);

    JGeometry::TVec3f getRailPos(const LiveActor *);

    bool isLoopRail(const LiveActor *);
    bool isRailReachedGoal(const LiveActor *);
    bool isRailReachedNearGoal(const LiveActor *, f32);

    bool getRailPointArg0NoInit(const LiveActor *, s32, s32 *);

    void calcRailPointPos(JGeometry::TVec3f *, const LiveActor *, s32);

    void calcRailStartPos(JGeometry::TVec3f *, const LiveActor *);
    void calcRailEndPos(JGeometry::TVec3f *, const LiveActor *);

    
};
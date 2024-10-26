#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.hpp"

class LiveActor;
class RailRider;

namespace MR {
    f32 getRailTotalLength(const LiveActor *);

    s32 getRailPointNum(const LiveActor *);
    s32 getCurrentRailPointNo(const LiveActor *);

    void calcRailPosAtCoord(TVec3f *, const LiveActor *, f32);

    void calcRailDirectionAtCoord(TVec3f *, const LiveActor *, f32);

    void calcRailPointPos(TVec3f *, const LiveActor *, int);
    f32 getRailCoord(const LiveActor *);
    f32 getRailPointCoord(const LiveActor *, int);

    bool isRailReachedGoal(const LiveActor *);

    void initAndSetRailClipping(TVec3f *, LiveActor *, f32, f32);

    void moveCoordToNearestPos(LiveActor *, const TVec3f &);

    void moveCoordAndTransToNearestRailPos(LiveActor *);
    void moveTransToCurrentRailPos(LiveActor *);

    void reverseRailDirection(LiveActor *);

    void setRailCoord(LiveActor *, f32);
    void setRailCoordSpeed(LiveActor *, f32);
    void accelerateRailCoordSpeed(LiveActor *, f32);

    void moveCoord(LiveActor *, f32);
    
    void moveTransToOtherActorRailPos(LiveActor *, const LiveActor *);

    const TVec3f& getRailPos(const LiveActor *);
    const TVec3f& getRailDirection(const LiveActor *);

    void moveCoordToStartPos(LiveActor *);

    bool getJMapInfoRailArg0NoInit(const LiveActor *, s32 *);
    bool getRailPointArg0NoInit(const LiveActor *, s32, f32 *);

    bool getRailPointArg0WithInit(const LiveActor *, s32, s32 *);

    bool getRailArg0WithInit(const LiveActor *, s32 *);
    bool getRailArg1WithInit(const RailRider *, s32 *);

    f64 calcNearestRailCoord(const LiveActor *, const TVec3f &);

    void moveCoordAndFollowTrans(LiveActor *, f32);

    bool isExistRail(const LiveActor *);

    void moveRailRider(LiveActor *);

    void moveCoordToEndPos(LiveActor *);

    void slowDownRailCoordSpeed(LiveActor *, f32);

    bool isRailGoingToEnd(const LiveActor *);

    f32 getRailCoordSpeed(const LiveActor *);

    void setRailDirectionToEnd(LiveActor *);

    bool isLoopRail(const LiveActor *);

    bool getCurrentRailPointArg0WithInit(const LiveActor *, s32 *);
    bool getCurrentRailPointArg1WithInit(const LiveActor *, s32 *);
    bool getCurrentRailPointArg0NoInit(const LiveActor *, s32 *);
    bool getCurrentRailPointArg1NoInit(const LiveActor *, s32 *);

    const TVec3f& getRailPointPosEnd(const LiveActor *);
    void calcRailEndPointPos(TVec3f *, const LiveActor *);
    void calcNearestRailPos(TVec3f *, const LiveActor *, const TVec3f &);
};

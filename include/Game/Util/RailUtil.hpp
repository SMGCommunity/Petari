#pragma once

#include "JSystem/JGeometry.hpp"
#include "JSystem/JGeometry/TBox.hpp"
#include <revolution.h>

class LiveActor;
class RailRider;

namespace MR {
    f32 getRailTotalLength(const LiveActor*);

    s32 getRailPointNum(const LiveActor*);
    s32 getCurrentRailPointNo(const LiveActor*);

    void calcRailPosAtCoord(TVec3f*, const LiveActor*, f32);

    void calcRailDirectionAtCoord(TVec3f*, const LiveActor*, f32);

    void calcRailPointPos(TVec3f*, const LiveActor*, int);
    void calcRailStartPointPos(TVec3f*, const LiveActor*);
    f32 getRailCoord(const LiveActor*);
    f32 getRailPointCoord(const LiveActor*, int);

    bool isRailReachedGoal(const LiveActor*);
    bool isRailReachedNearGoal(const LiveActor*, f32);

    void initAndSetRailClipping(TVec3f*, LiveActor*, f32, f32);

    void moveCoordToNearestPos(LiveActor*, const TVec3f&);

    s32 moveCoordAndCheckPassPointNo(LiveActor*, f32);
    void moveCoordAndTransToNearestRailPos(LiveActor*);
    void moveCoordAndTransToNearestRailPos(LiveActor*, const TVec3f&);
    void moveCoordAndTransToRailPoint(LiveActor*, s32);
    void moveTransToCurrentRailPos(LiveActor*);

    void reverseRailDirection(LiveActor*);

    void setRailCoord(LiveActor*, f32);
    void setRailCoordSpeed(LiveActor*, f32);
    void accelerateRailCoordSpeed(LiveActor*, f32);

    void calcDistanceToCurrentAndNextRailPoint(const LiveActor*, f32*, f32*);

    void moveCoord(LiveActor*, f32);

    void moveCoordToRailPoint(LiveActor*, s32);

    s32 getNextRailPointNo(const LiveActor*);

    void moveTransToOtherActorRailPos(LiveActor*, const LiveActor*);

    const TVec3f& getRailPos(const LiveActor*);
    const TVec3f& getRailDirection(const LiveActor*);

    const TVec3f& getRailPointPosStart(const LiveActor*);

    void moveCoordToStartPos(LiveActor*);
    void moveCoordAndTransToRailStartPoint(LiveActor*);

    void moveCoordAndTransToNextPoint(LiveActor*);

    bool getJMapInfoRailArg0NoInit(const LiveActor*, s32*);
    bool getRailPointArg0NoInit(const LiveActor*, s32, s32*);
    bool getRailPointArg0NoInit(const LiveActor*, s32, f32*);

    void reverseRailDirection(LiveActor*);

    void setRailCoord(LiveActor*, f32);
    void setRailCoordSpeed(LiveActor*, f32);
    void accelerateRailCoordSpeed(LiveActor*, f32);

    void calcDistanceToCurrentAndNextRailPoint(const LiveActor*, f32*, f32*);

    void moveCoord(LiveActor*, f32);

    void moveTransToOtherActorRailPos(LiveActor*, const LiveActor*);

    const TVec3f& getRailPos(const LiveActor*);
    const TVec3f& getRailDirection(const LiveActor*);

    const TVec3f& getRailPointPosStart(const LiveActor*);

    void moveCoordToStartPos(LiveActor*);
    void moveCoordAndTransToRailStartPoint(LiveActor*);

    bool getJMapInfoRailArg0NoInit(const LiveActor*, s32*);
    bool getRailPointArg0NoInit(const LiveActor*, s32, f32*);

    bool getRailPointArg0WithInit(const LiveActor*, s32, s32*);

    bool getRailArg0WithInit(const LiveActor*, s32*);
    bool getRailArg1WithInit(const RailRider*, s32*);

    bool getRailArg3NoInit(const LiveActor*, s32*);
    void setRailDirectionToStart(LiveActor*);
    void setRailDirectionToEnd(LiveActor*);
    void setRailDirectionCloseToNearestPos(LiveActor*, const TVec3f&);

    f64 calcNearestRailCoord(const LiveActor*, const TVec3f&);

    void moveCoordAndFollowTrans(LiveActor*);
    void moveCoordAndFollowTrans(LiveActor*, f32);

    bool isExistRail(const LiveActor*);

    void moveRailRider(LiveActor*);

    void moveCoordToEndPos(LiveActor*);

    void slowDownRailCoordSpeed(LiveActor*, f32);

    bool isRailGoingToEnd(const LiveActor*);

    f32 getRailCoordSpeed(const LiveActor*);

    void setRailDirectionToEnd(LiveActor*);
    void setRailDirectionCloseToNearestPos(LiveActor*, const TVec3f&);

    bool isLoopRail(const LiveActor*);

    bool getRailPointArg1NoInit(const LiveActor*, s32, f32*);

    bool getCurrentRailPointArg0WithInit(const LiveActor*, s32*);
    bool getCurrentRailPointArg1WithInit(const LiveActor*, s32*);
    bool getCurrentRailPointArg0NoInit(const LiveActor*, s32*);
    bool getCurrentRailPointArg1NoInit(const LiveActor*, s32*);

    bool getCurrentRailPointArg0NoInit(const LiveActor*, f32*);

    bool getCurrentRailPointArg1NoInit(const LiveActor*, f32*);

    bool getNextRailPointArg0NoInit(const LiveActor*, f32*);
    bool getNextRailPointArg1NoInit(const LiveActor*, f32*);

    const TVec3f& getRailPointPosEnd(const LiveActor*);
    void calcRailEndPointPos(TVec3f*, const LiveActor*);
    void calcRailEndPointDirection(TVec3f*, const LiveActor*);
    void calcNearestRailDirection(TVec3f*, const LiveActor*, const TVec3f&);
    void calcNearestRailPos(TVec3f*, const LiveActor*, const TVec3f&);
    void calcNearestRailPosAndDirection(TVec3f*, TVec3f*, const LiveActor*, const TVec3f&);

    void calcMovingDirectionAlongRail(LiveActor*, TVec3f*, const TVec3f&, float, bool, bool*);
    void calcMovingDirectionAlongRailH(LiveActor*, TVec3f*, const TVec3f&, float, bool*);

    void calcRailPosAndDirectionAtCoord(TVec3f*, TVec3f*, const LiveActor*, f32);
    void calcRailStartPos(TVec3f*, const LiveActor*);

    void calcBoundingBox(const LiveActor*, TBox3f*, f32);

    void calcRailClippingInfo(TVec3f*, f32*, const LiveActor*, f32, f32);
};  // namespace MR

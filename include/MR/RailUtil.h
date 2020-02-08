#ifndef RAILUTIL_H
#define RAILUTIL_H

#include "JGeometry/TVec3.h"
#include <revolution.h>

class LiveActor;

namespace MR
{
    void moveCoordToStartPos(LiveActor *);
    void moveCoordToEndPos(LiveActor *);

    bool isExistRail(const LiveActor *);
    s32 getRailPointNum(const LiveActor *);
    s32 getCurrentRailPointNo(const LiveActor *);

    f32 getRailTotalLength(const LiveActor *);
    JGeometry::TVec3<f32> getRailDirection(const LiveActor *);
    JGeometry::TVec3<f32> getRailCoord(const LiveActor *);
    f32 getRailCoordSpeed(const LiveActor *);
    void setRailCoord(LiveActor *, f32);
    void setRailCoordSpeed(LiveActor *, f32);

    JGeometry::TVec3<f32> getRailPos(const LiveActor *);

    void calcRailClippingInfo(JGeometry::TVec3<f32> *, f32 *, const LiveActor *, f32, f32);

    bool isLoopRail(const LiveActor *);
    bool isRailReachedGoal(const LiveActor *);
    bool isRailReachedNearGoal(const LiveActor *, f32);

    bool getRailPointArg0NoInit(const LiveActor *, s32, s32 *);

    void calcRailPointPos(JGeometry::TVec3<f32> *, const LiveActor *, s32);

    void calcRailStartPos(JGeometry::TVec3<f32> *, const LiveActor *);
    void calcRailEndPos(JGeometry::TVec3<f32> *, const LiveActor *);

    f32 calcNearestRailPos(JGeometry::TVec3<f32> *, const LiveActor *, const JGeometry::TVec3<f32> &);
};

#endif // RAILUTIL_H
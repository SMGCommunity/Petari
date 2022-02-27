#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class LiveActor;

namespace MR {
    f32 getRailTotalLength(const LiveActor *);

    s32 getRailPointNum(const LiveActor *);
    s32 getCurrentRailPointNo(const LiveActor *);

    void calcRailPosAtCoord(TVec3f *, const LiveActor *, f32);

    void calcRailPointPos(TVec3f *, const LiveActor *, int);
    f32 getRailCoord(const LiveActor *);
    f32 getRailPointCoord(const LiveActor *, int);

    bool isRailReachedGoal(const LiveActor *);

    void initAndSetRailClipping(TVec3f *, LiveActor *, f32, f32);

    void moveCoordToNearestPos(LiveActor *, const TVec3f &);

    void moveTransToCurrentRailPos(LiveActor *);

    void reverseRailDirection(LiveActor *);

    void setRailCoord(LiveActor *, f32);

    void moveCoord(LiveActor *, f32);
    
    void moveTransToOtherActorRailPos(LiveActor *, const LiveActor *);

    const TVec3f& getRailDirection(const LiveActor *);
};
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
};
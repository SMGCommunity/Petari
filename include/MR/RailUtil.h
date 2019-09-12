#ifndef RAILUTIL_H
#define RAILUTIL_H

#include "JGeometry/TVec3.h"
#include "types.h"

class LiveActor;

namespace MR
{
    s32 getRailPointNum(const LiveActor *);

    void calcRailClippingInfo(JGeometry::TVec3<f32> *, f32 *, const LiveActor *, f32, f32);

    bool getRailPointArg0NoInit(const LiveActor *, s32, s32 *);

    void calcRailPointPos(JGeometry::TVec3<f32> *, const LiveActor *, s32);
};

#endif // RAILUTIL_H
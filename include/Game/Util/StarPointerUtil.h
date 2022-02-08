#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class LiveActor;

namespace MR {
    void initStarPointerTarget(LiveActor *, f32, const TVec3f &);
    void initStarPointerTargetAtPos(LiveActor *, f32, const TVec3f *, const TVec3f &);
    void initStarPointerTargetAtMtx(LiveActor *, f32, MtxPtr, const TVec3f &);
    void initStarPointerTargetAtJoint(LiveActor *, const char *, f32, const TVec3f &);

    f32 calcPointRadius2D(const TVec3f &, f32);
};
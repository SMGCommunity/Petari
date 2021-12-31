#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.h"

class LiveActor;

namespace MR {
    f32 getRailTotalLength(const LiveActor *);

    void calcRailPosAtCoord(TVec3f *, const LiveActor *, f32);
};
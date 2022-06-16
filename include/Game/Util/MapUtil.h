#pragma once

#include "JSystem/JGeometry.h"
#include "Game/Util.h"

class LiveActor;
class Triangle;
class TriangleFilterBase;

namespace MR {
    bool isBindedGroundDamageFire(const LiveActor *);

    bool getFirstPolyOnLineToMapExceptActor(TVec3f * , Triangle *, const TVec3f &, const TVec3f &, const LiveActor *);

    bool isWallCodeNoAction(const Triangle *);

    void setBindTriangleFilter(LiveActor *, TriangleFilterBase *);
};

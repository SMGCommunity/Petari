#pragma once

#include "JSystem/JGeometry.h"
#include "Game/Util.h"

class LiveActor;
class Triangle;

namespace MR {
    bool isBindedGroundDamageFire(const LiveActor *);

    bool getFirstPolyOnLineToMapExceptActor(TVec3f * , Triangle *, const TVec3f &, const TVec3f &, const LiveActor *);
};
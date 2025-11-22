#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class LiveActor;

class TerritoryMover {
public:
    TerritoryMover(f32);

    void decideNextTargetPos(const LiveActor*);
    bool isReachedTarget(const LiveActor*, f32);

    f32 _0;
    TVec3f _4;
    TVec3f _10;
};

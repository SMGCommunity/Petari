#pragma once

#include <revolution.h>
#include <JSystem/JGeometry.hpp>

class LiveActor;

class TerritoryMover {
public:
    TerritoryMover(f32);

    void decideNextTargetPos(const LiveActor*);
    bool isReachedTarget(const LiveActor*, f32);

    f32    _0;
    TVec3f _4;
    TVec3f _10;
};

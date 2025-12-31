#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class LiveActor;

class TerritoryMover {
public:
    TerritoryMover(f32);

    void decideNextTargetPos(const LiveActor*);
    bool isReachedTarget(const LiveActor*, f32);

    void setCenter(const TVec3f& rPos) { mCenter.set(rPos); }

    f32 _0;
    TVec3f mCenter;  // 0x4
    TVec3f _10;
};

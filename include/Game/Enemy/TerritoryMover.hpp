#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class LiveActor;

class TerritoryMover {
public:
    TerritoryMover(f32);

    void decideNextTargetPos(const LiveActor*);
    bool isReachedTarget(const LiveActor*, f32);

    void setCenter(const TVec3f& rPos) NO_INLINE {
        mCenter.set(rPos);
    }

    /* 0x00 */ f32 mRadius;
    /* 0x04 */ TVec3f mCenter;
    /* 0x10 */ TVec3f mTarget;
};

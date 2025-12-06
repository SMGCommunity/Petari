#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TripodBossStepPoint : public LiveActor {
public:
    TripodBossStepPoint(const char*);

    virtual ~TripodBossStepPoint();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();

    void setStepPosition(const TVec3f&);
    void setStepNormal(const TVec3f&);
    void setStepFront(const TVec3f&);

    TVec3f mStepPosition;  // 0x8C
    TVec3f mStepNormal;    // 0x98
    TVec3f mStepFront;     // 0xA4
    s32 mLeg;              // 0xB0
    s32 _B4;
    s32 _B8;
    s32 mWaitTime;  // 0xBC
};

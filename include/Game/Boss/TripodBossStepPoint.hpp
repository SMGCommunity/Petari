#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class TripodBossStepPoint : public LiveActor {
public:
    /// @brief Creates a new `TripodBossStepPoint`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossStepPoint(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();

    void setStepPosition(const TVec3f&);
    void setStepNormal(const TVec3f&);
    void setStepFront(const TVec3f&);

    /* 0x8C */ TVec3f mStepPosition;
    /* 0x98 */ TVec3f mStepNormal;
    /* 0xA4 */ TVec3f mStepFront;
    /* 0xB0 */ s32 mLeg;
    /* 0xB4 */ s32 mSequencePriority;
    /* 0xB8 */ s32 mArg3;
    /* 0xBC */ s32 mWaitTime;
};

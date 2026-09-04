#pragma once

#include <revolution/types.h>

class TripodBossStepPoint;

class TripodBossStepSequence {
public:
    /// @brief Creates a new `TripodBossStepSequence`.
    TripodBossStepSequence();

    void addStepPoint(TripodBossStepPoint*);
    s32 getCurrentLeg() const;
    s32 getCurrentWaitTime() const;
    TripodBossStepPoint* getCurrentStepPoint();
    void nextStep();
    bool isEmpty() const;
    void reset();

    /* 0x00 */ TripodBossStepPoint* mStepPoints[32];
    /* 0x80 */ s32 mCurrentPoint;
    /* 0x84 */ s32 mPointNum;
    /* 0x88 */ bool _88;
};

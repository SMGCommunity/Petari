#pragma once

#include <revolution.h>

class TripodBossStepPoint;

class TripodBossStepSequence {
public:
    TripodBossStepSequence();

    void addStepPoint(TripodBossStepPoint*);
    s32 getCurrentLeg() const;
    s32 getCurrentWaitTime() const;
    TripodBossStepPoint* getCurrentStepPoint();
    void nextStep();
    bool isEmpty() const;
    void reset();

    TripodBossStepPoint* mStepPoints[0x20];  // 0x00
    s32 mCurrentPoint;                       // 0x80
    s32 mPointNum;                           // 0x84
    u8 _88;
};

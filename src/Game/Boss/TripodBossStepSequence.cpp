#include "Game/Boss/TripodBossStepSequence.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"

TripodBossStepSequence::TripodBossStepSequence() {
    mCurrentPoint = 0;
    _84 = 0;
    _88 = 1;

    for (s32 i = 0; i < 0x20; i++) {
        mStepPoints[i] = nullptr;
    }
}

// https://decomp.me/scratch/r7kc5
// not really sure what this is doing
void TripodBossStepSequence::addStepPoint(TripodBossStepPoint *pPoint) {
    for (s32 i = 0; i < _84; i++) {
        if (mStepPoints[i] == nullptr) {
            mStepPoints[i] = pPoint;
            return;
        }
    }
}

s32 TripodBossStepSequence::getCurrentLeg() const {
    return mStepPoints[mCurrentPoint]->mLeg;
}

s32 TripodBossStepSequence::getCurrentWaitTime() const {
    return mStepPoints[mCurrentPoint]->mWaitTime;
}

TripodBossStepPoint* TripodBossStepSequence::getCurrentStepPoint() {
    return mStepPoints[mCurrentPoint];
}

void TripodBossStepSequence::nextStep() {
    mCurrentPoint++;
    if (mCurrentPoint >= _84) {
        mCurrentPoint = 0;
    }
}

bool TripodBossStepSequence::isEmpty() const {
    return mStepPoints[mCurrentPoint] == nullptr;
}

void TripodBossStepSequence::reset() {
    mCurrentPoint = 0;
}

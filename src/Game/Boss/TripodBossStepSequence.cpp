#include "Game/Boss/TripodBossStepSequence.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"

TripodBossStepSequence::TripodBossStepSequence() : mCurrentPoint(), mPointNum(), _88(true) {
    for (s32 i = 0; i < ARRAY_SIZE(mStepPoints); i++) {
        mStepPoints[i] = nullptr;
    }
}

void TripodBossStepSequence::addStepPoint(TripodBossStepPoint* pPoint) {
    s32 sortPriority = pPoint->mSequencePriority;

    mPointNum++;

    for (s32 i = mPointNum - 1; i > 0; i--) {
        if (mStepPoints[i - 1]->mSequencePriority < sortPriority) {
            mStepPoints[i] = pPoint;
            return;
        }

        mStepPoints[i] = mStepPoints[i - 1];
    }

    mStepPoints[0] = pPoint;
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

    if (mCurrentPoint >= mPointNum) {
        mCurrentPoint = 0;
    }
}

bool TripodBossStepSequence::isEmpty() const {
    return mStepPoints[mCurrentPoint] == nullptr;
}

void TripodBossStepSequence::reset() {
    mCurrentPoint = 0;
}

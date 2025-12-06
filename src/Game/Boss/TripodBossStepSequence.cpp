#include "Game/Boss/TripodBossStepSequence.hpp"
#include "Game/Boss/TripodBossStepPoint.hpp"

TripodBossStepSequence::TripodBossStepSequence() {
    mCurrentPoint = 0;
    mPointNum = 0;
    _88 = 1;

    for (s32 i = 0; i < 0x20; i++) {
        mStepPoints[i] = nullptr;
    }
}

void TripodBossStepSequence::addStepPoint(TripodBossStepPoint* pPoint) {
    int temp = pPoint->_B4;
    int pointNum = mPointNum++;

    for (int i = mPointNum - 1; i > 0; i--) {
        if (mStepPoints[i - 1]->_B4 < temp) {
            mStepPoints[i] = pPoint;
            return;
        } else {
            mStepPoints[i] = mStepPoints[i - 1];
        }
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

#include "Game/Camera/CameraShakeTask.hpp"
#include "Game/Camera/CameraShakePatternImpl.hpp"

CameraShakeTask::CameraShakeTask(CameraShakePattern* pPattern)
    : mPattern(pPattern), mHasEnded(true), mIsInfinite(), mTime(), mDelay(), mDelayTimer() {
}

void CameraShakeTask::start(u32 time, u32 delay) {
    mTime = time;
    mIsInfinite = false;

    startCommon(delay);
}

void CameraShakeTask::startInfinity(u32 delay) {
    mIsInfinite = true;

    startCommon(delay);
}

void CameraShakeTask::endForce() {
    mHasEnded = true;
}

void CameraShakeTask::movement() {
    if (!mHasEnded) {
        if (!mPattern->isEnd()) {
            updatePattern();
        }

        if (mPattern->isEnd()) {
            updateInterval();
        }
    }
}

void CameraShakeTask::getOffset(TVec2f* pOffset) const {
    if (mHasEnded || mPattern->isEnd()) {
        pOffset->set(0.0f, 0.0f);
    } else {
        mPattern->getOffset(pOffset);
    }
}

bool CameraShakeTask::isEnd() const {
    return mHasEnded;
}

void CameraShakeTask::startCommon(u32 delay) {
    mDelay = delay;
    CameraShakePattern* pattern = mPattern;
    mHasEnded = false;
    pattern->mFrame = 0;
    pattern->start();
}

void CameraShakeTask::updatePattern() {
    CameraShakePattern* pattern = mPattern;

    pattern->mFrame++;
    pattern->update();

    if (mPattern->isEnd()) {
        if (!mIsInfinite) {
            mTime--;
        }

        mDelayTimer = 0;
    }
}

void CameraShakeTask::updateInterval() {
    if (mIsInfinite || mTime != 0) {
        if (++mDelayTimer < mDelay) {
            return;
        }

        CameraShakePattern* pattern = mPattern;

        pattern->mFrame = 0;
        pattern->start();
    } else {
        mHasEnded = true;
    }
}

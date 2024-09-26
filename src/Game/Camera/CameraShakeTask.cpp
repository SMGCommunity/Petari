#include "Game/Camera/CameraShakePatternImpl.hpp"
#include "Game/Camera/CameraShakeTask.hpp"

CameraShakeTask::CameraShakeTask(CameraShakePattern *pPattern) {
    mPattern = pPattern;
    mHasEnded = true;
    mIsInfinite = false;
    _8 = 0;
    _C = 0;
    _10 = 0;
}

void CameraShakeTask::start(unsigned long a1, unsigned long a2) {
    _8 = a1;
    mIsInfinite = false;

    startCommon(a2);
}

void CameraShakeTask::startInfinity(unsigned long a1) {
    mIsInfinite = true;

    startCommon(a1);
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

void CameraShakeTask::getOffset(TVec2f *pOffset) const {
    if (mHasEnded || mPattern->isEnd()) {
        pOffset->x = 0.0f;
        pOffset->y = 0.0f;
    }
    else {
        mPattern->getOffset(pOffset);
    }
}

bool CameraShakeTask::isEnd() const {
    return mHasEnded;
}

void CameraShakeTask::startCommon(unsigned long a1) {
    _C = a1;
    CameraShakePattern *pattern = mPattern;
    mHasEnded = false;
    pattern->_4 = 0;
    pattern->start();
}

void CameraShakeTask::updatePattern() {
    CameraShakePattern *pattern = mPattern;

    pattern->_4++;    
    pattern->update();

    if (mPattern->isEnd()) {
        if (!mIsInfinite) {
            _8--;
        }

        _10 = 0;
    }
}

void CameraShakeTask::updateInterval() {
    if (mIsInfinite || _8 != 0) {
        if (++_10 < _C) {
            return;
        }

        CameraShakePattern *pattern = mPattern;

        pattern->_4 = 0;
        pattern->start();
    }
    else {
        mHasEnded = true;
    }
}
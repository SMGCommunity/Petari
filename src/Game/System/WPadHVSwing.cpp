#include "Game/System/WPadHVSwing.hpp"
#include "Game/System/WPad.hpp"

WPadHVSwing::WPadHVSwing(const WPad* pPad, u32 channel)
    : mPad(pPad), mChannel(channel), mDistanceSwingThreshold(1.0f), mIsSwing(), mIsSwingLatched(), mSwingLatchedFrames(),
      mSwingBelowThresholdFrames(), mIsTriggerSwing(), mSwingThreshold(0.4f), mSwingDetected(), mSwingTriggered(), mSwingHoldFrames(),
      mSwingCooldownFrames() {
}

void WPadHVSwing::updateSwing() {
    TVec3f pastAccel;
    TVec3f curAccel;

    if (!mPad->getPastAcceleration(&pastAccel, 20, mChannel) || !mPad->getAcceleration(&curAccel, mChannel)) {
        mIsSwing = false;
        return;
    }

    mIsSwing = curAccel.distance(pastAccel) >= mDistanceSwingThreshold;

    if (!mIsSwing) {
        mSwingBelowThresholdFrames++;
    } else {
        mSwingBelowThresholdFrames = 0;
    }

    if (mIsSwingLatched) {
        if (mSwingBelowThresholdFrames > 6) {
            mIsSwingLatched = false;
        }
    } else if (mIsSwing) {
        mIsSwingLatched = true;
    }

    if (mIsSwingLatched) {
        mSwingLatchedFrames++;
    } else {
        mSwingLatchedFrames = 0;
    }
}

void WPadHVSwing::updateCentrifugal() {
    TVec3f pastAccel;
    TVec3f curAccel;

    if (!mPad->getPastAcceleration(&pastAccel, 20, mChannel) || !mPad->getAcceleration(&curAccel, mChannel)) {
        mSwingDetected = false;
        mSwingTriggered = false;
        mSwingHoldFrames = 0;
        mSwingCooldownFrames = 0;
    }

    float accelDeltaSum = 0.0f;
    float peakDelta = 0.0f;

    if (mPad->getEnableAccelPastCount(mChannel) >= 15) {
        for (int i = 1; i < 15; i++) {
            TVec3f prevSample;
            mPad->getPastAcceleration(&prevSample, i - 1, mChannel);

            TVec3f currSample;
            mPad->getPastAcceleration(&currSample, i, mChannel);

            accelDeltaSum += (prevSample.y - currSample.y);

            if (peakDelta < accelDeltaSum) {
                peakDelta = accelDeltaSum;
            }
        }
    }

    mSwingDetected = peakDelta > mSwingThreshold;

    if (!mSwingDetected) {
        mSwingCooldownFrames++;
    } else {
        mSwingCooldownFrames = 0;
    }

    if (mSwingTriggered) {
        if (mSwingCooldownFrames > 8) {
            mSwingTriggered = false;
        }
    } else if (mSwingDetected) {
        mSwingTriggered = true;
    }

    if (mSwingTriggered) {
        mSwingHoldFrames++;
    } else {
        mSwingHoldFrames = 0;
    }
}

void WPadHVSwing::update() {
    updateSwing();
    updateCentrifugal();

    mIsTriggerSwing = false;

    if (mSwingLatchedFrames == 1) {
        mIsTriggerSwing = true;
    } else if (mSwingHoldFrames >= 30) {
        bool isOffPulseBoundary = (mSwingHoldFrames - 30) % 15 != 0;

        if (!isOffPulseBoundary && mSwingCooldownFrames < 15) {
            mIsTriggerSwing = true;
        }
    }
}

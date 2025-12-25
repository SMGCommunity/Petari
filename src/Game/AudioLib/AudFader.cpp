#include "Game/AudioLib/AudFader.hpp"

AudFader::AudFader() : mCurrentVolume(1.f), mFinalVolume(1.f), mStepVolume(0.f) {}

void AudFader::set(float desiredVolume, long fadeTime) {
    if (fadeTime == 0) {
        mCurrentVolume = desiredVolume;
        mStepVolume = 0.f;
    } else {
        mStepVolume = (desiredVolume - mCurrentVolume) / (float)(fadeTime);
    }
    mFinalVolume = desiredVolume;
}

void AudFader::update() {
    if (mStepVolume == 0.0f) {
        return;
    }

    mCurrentVolume += mStepVolume;

    if (mStepVolume > 0.0f) {
        if (mCurrentVolume >= mFinalVolume) {
            mCurrentVolume = mFinalVolume;
            mStepVolume = 0.0f;
        }
    } else {
        if (mCurrentVolume <= mFinalVolume) {
            mCurrentVolume = mFinalVolume;
            mStepVolume = 0.f;
        }
    }
}

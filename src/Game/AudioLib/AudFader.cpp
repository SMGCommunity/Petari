#include "Game/AudioLib/AudFader.hpp"

AudFader::AudFader() : mCurrentVolume(1.f), mFinalVolume(1.f), mStepVolume(0.f) {}

/// @brief Initializes AudFader
/// @param desiredVolume Volume intensity to be reached at the end of fade
/// @param fadeTime Time/Steps to reach desiredVolume intensity
void AudFader::set(float desiredVolume, long fadeTime) {
    if (fadeTime == 0) {
        // Set desiredVolume instantly
        mCurrentVolume = desiredVolume;
        mStepVolume = 0.f;
    } else {
        mStepVolume = (desiredVolume - mCurrentVolume) / (float)(fadeTime);
    }
    mFinalVolume = desiredVolume;
}

/// @brief Updates volume intensity for fade
void AudFader::update() {
    // No need to update, mFinalVolume has been reached
    if (mStepVolume == 0.0f) {
        return;
    }

    mCurrentVolume += mStepVolume;

    if (mStepVolume > 0.0f) {
        // Clamp audio intensity when mCurrentVolume reaches mFinalVolume in fade up
        if (mCurrentVolume >= mFinalVolume) {
            mCurrentVolume = mFinalVolume;
            mStepVolume = 0.0f;
        }
    } else {
        // Clamp audio intensity when mCurrentVolume reaches mFinalVolume in fade down
        if (mCurrentVolume <= mFinalVolume) {
            mCurrentVolume = mFinalVolume;
            mStepVolume = 0.f;
        }
    }
}

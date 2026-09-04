#include "JSystem/JAudio2/JAUAudience.hpp"

void JAUAudience3DSetting::init() {
    mFarDistance = 7000.0f;
    mNearDistance = 800.0f;
    mMinVolume = 0.005f;
    mBehindDolbyZ = 8000.0f;
    mFrontDolbyZ = -4000.0f;
    mMaxFxMix = 0.1f;
    mMinFxMix = 0.0f;
    mMaxDolby = 1.0f;
    mCenterDolby = 0.8f;
    mMinDolby = 0.0f;
    mPanAmplitude = 0.5f;
    mPriorityDownRange = 255;
    mSonicSpeed = 3000.0f;
    mPitchDeltaRatioRange = 1.5f;
}

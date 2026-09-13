#include "JSystem/JAudio2/JAUAudience.hpp"
#include "JSystem/JAudio2/JAUAudible.hpp"

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

// NOTE: Dummies to emit sinit and ctor/dtor pattern
JAUAudible< 1 >* newAudible_1() {
    return new JAUAudible< 1 >();
}

JAUAudible< 2 >* newAudible_2() {
    return new JAUAudible< 2 >();
}

JAUAudible< 4 >* newAudible_4() {
    return new JAUAudible< 4 >();
}

JAUDopplerAudible< 1 >* newDopplerAudible_1() {
    return new JAUDopplerAudible< 1 >();
}

JAUDopplerAudible< 2 >* newDopplerAudible_2() {
    return new JAUDopplerAudible< 2 >();
}

JAUDopplerAudible< 4 >* newDopplerAudible_4() {
    return new JAUDopplerAudible< 4 >();
}

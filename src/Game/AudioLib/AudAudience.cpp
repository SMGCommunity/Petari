#include "Game/AudioLib/AudAudience.hpp"

void AudAudience_withSetting::updateSetting() {
    mFrontDolbyRate = (mSetting.mMinDolby - mSetting.mCenterDolby) / mSetting.mFrontDolbyZ;
    mBehindDolbyRate = (mSetting.mMaxDolby - mSetting.mMinDolby) / mSetting.mBehindDolbyZ;
    mVolumeRate = (mSetting.mMinVolume - 1.0f) / (mSetting.mFarDistance - mSetting.mNearDistance);
    mPriorityRate = mSetting.mPriorityDownRange / (mSetting.mFarDistance - mSetting.mNearDistance);
    mFxMixRate = (mSetting.mMaxFxMix - mSetting.mMinFxMix) / (mSetting.mFarDistance - mSetting.mNearDistance);
}

AudAudience_withSetting::AudAudience_withSetting() {
    updateSetting();

    mFarCamera = false;
}

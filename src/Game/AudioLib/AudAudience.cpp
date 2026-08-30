#include "Game/AudioLib/AudAudience.hpp"

void AudAudience_withSetting::updateSetting() {
    _3C = (mSetting.mMinDolby - mSetting.mCenterDolby) / mSetting.mFrontDolbyZ;
    _40 = (mSetting.mMaxDolby - mSetting.mCenterDolby) / mSetting.mBehindDolbyZ;

    f32 diff = mSetting.mFarDistance - mSetting.mNearDistance;

    _44 = (mSetting.mMinVolume - 1.0f) / diff;
    _48 = mSetting.mPriorityDownRange / diff;
    _4C = (mSetting._24 - mSetting._20) / diff;
}

AudAudience_withSetting::AudAudience_withSetting() {
    updateSetting();

    mFarCamera = false;
}

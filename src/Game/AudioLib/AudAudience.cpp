#include "Game/AudioLib/AudAudience.hpp"

AudAudience_withSetting::AudAudience_withSetting() {
    updateSetting();

    mFarCamera = false;
}

void AudAudience_withSetting::updateSetting() {
    _3C = (mSetting._1C - mSetting._18) / mSetting._C;
    _40 = (mSetting._14 - mSetting._18) / mSetting._10;

    f32 diff = mSetting._0 - mSetting._4;

    _44 = (mSetting._8 - 1.0f) / diff;
    _48 = mSetting._34 / diff;
    _4C = (mSetting._24 - mSetting._20) / diff;
}

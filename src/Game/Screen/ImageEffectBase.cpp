#include "Game/Screen/ImageEffectBase.hpp"

namespace {
    const f32 sFadeCounterMax = 15.0f;
};

ImageEffectBase::ImageEffectBase(const char* pName) : NameObj(pName), mIsActive(), mIsFade(), mFadeIntensity() {
}

void ImageEffectBase::calcAnim() {
    if (mIsActive) {
        mIsFade = true;
        mFadeIntensity += 1.0f / sFadeCounterMax;

        if (mFadeIntensity > 1.0f) {
            mFadeIntensity = 1.0f;
        }
    } else if (isVisible()) {
        mFadeIntensity -= 1.0f / sFadeCounterMax;

        if (mFadeIntensity < 0.0f) {
            mIsFade = false;
            mFadeIntensity = 0.0f;
        }
    }

    calcAnimSub();
}

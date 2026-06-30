#include "Game/AreaObj/DepthOfFieldArea.hpp"
#include "Game/Util/ScreenUtil.hpp"

void DepthOfFieldArea_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

DepthOfFieldArea::DepthOfFieldArea(int formType, const char* pName) : ImageEffectArea(IMAGE_EFFECT_TYPE_DOF, formType, pName), mIsPlayerSync() {
}

void DepthOfFieldArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);

    mIsPlayerSync = mObjArg0 > 0;

    MR::createDepthOfFieldBlur();
}

f32 DepthOfFieldArea::getIntensity() const {
    if (mObjArg1 > -1) {
        return mObjArg1 / 255.0f;
    }

    return 1.0f;
}

s32 DepthOfFieldArea::getBlurMaxDist() const {
    return mObjArg2;
}

s32 DepthOfFieldArea::getBlurMinDist() const {
    return mObjArg3;
}

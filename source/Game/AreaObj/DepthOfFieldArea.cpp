#include "Game/AreaObj/DepthOfFieldArea.hpp"
#include "Game/Util/ScreenUtil.hpp"

DepthOfFieldArea::DepthOfFieldArea(int type, const char *pName) :
    ImageEffectArea(IMAGE_EFFECT_TYPE_DOF, type, pName) {
    mIsPlayerSync = false;
}

DepthOfFieldArea::~DepthOfFieldArea() {

}

void DepthOfFieldArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mIsPlayerSync = mObjArg0 > 0;
    MR::createDepthOfFieldBlur();
}

const char *DepthOfFieldArea::getManagerName() const {
    return "ImageEffectArea";
}

f32 DepthOfFieldArea::getIntensity() const {
    if (mObjArg1 > -1) {
        return static_cast<f32>(mObjArg1) / 255.0f;
    }

    return 1.0f;
}

s32 DepthOfFieldArea::getBlurMaxDist() const {
    return mObjArg2;
}

s32 DepthOfFieldArea::getBlurMinDist() const {
    return mObjArg3;
}

bool DepthOfFieldArea::isSyncWithPlayer() const {
    return mIsPlayerSync;
}

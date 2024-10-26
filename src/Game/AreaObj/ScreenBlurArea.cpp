#include "Game/AreaObj/ScreenBlurArea.hpp"
#include "Game/Util/ScreenUtil.hpp"

ScreenBlurArea::ScreenBlurArea(int type, const char *pName) :
    ImageEffectArea(IMAGE_EFFECT_TYPE_BLUR, type, pName) {
    mIsPlayerSync = false;
}

void ScreenBlurArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mIsPlayerSync = mObjArg0 > 0;
    MR::createScreenBlur();
}

ScreenBlurArea::~ScreenBlurArea() {

}

bool ScreenBlurArea::isSyncWithPlayer() const {
    return mIsPlayerSync;
}

const char *ScreenBlurArea::getManagerName() const {
    return "ImageEffectArea";
}

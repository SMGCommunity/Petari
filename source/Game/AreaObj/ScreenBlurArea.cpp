#include "Game/AreaObj/ScreenBlurArea.h"

ScreenBlurArea::ScreenBlurArea(int type, const char *pName) :
    ImageEffectArea(IMAGE_EFFECT_TYPE_BLUR, type, pName) {
    mIsPlayerSync = false;
}

ScreenBlurArea::~ScreenBlurArea() {

}

void ScreenBlurArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    mIsPlayerSync = mObjArg0 > 0;
    MR::createScreenBlur();
}

const char *ScreenBlurArea::getManagerName() const {
    return "ImageEffectArea";
}

bool ScreenBlurArea::isSyncWithPlayer() const {
    return mIsPlayerSync;
}

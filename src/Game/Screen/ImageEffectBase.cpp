#include "Game/Screen/ImageEffectBase.hpp"

void ImageEffectBase_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

ImageEffectBase::ImageEffectBase(const char* pName) : NameObj(pName), _C(), _D(), _10() {
}

void ImageEffectBase::calcAnim() {
    if (_C) {
        _D = true;
        _10 += 1.0f / 15.0f;

        if (_10 > 1.0f) {
            _10 = 1.0f;
        }
    } else if (isSomething()) {
        _10 -= 1.0f / 15.0f;

        if (_10 < 0.0f) {
            _D = false;
            _10 = 0.0f;
        }
    }

    calcAnimSub();
}

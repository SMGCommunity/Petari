#include "Game/Screen/ImageEffectBase.hpp"

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

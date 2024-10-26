#include "Game/AreaObj/BloomArea.hpp"
#include "Game/Util/ScreenUtil.hpp"

BloomArea::BloomArea(int type, const char *pName) :
    ImageEffectArea(IMAGE_EFFECT_TYPE_BLOOM, type, pName) {
    _40 = 128;
    _41 = 255;
    _44 = -1;
    _48 = -1;
}

BloomArea::~BloomArea() {

}

void BloomArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::createNormalBloom();

    if (mObjArg0 >= 0) {
        _41 = static_cast<u8>(mObjArg0);
    }

    if (mObjArg1 >= 0) {
        _40 = static_cast<u8>(mObjArg1);
    }

    if (mObjArg2 >= 0) {
        _44 = mObjArg2;
    }

    if (mObjArg3 >= 0) {
        _48 = mObjArg3;
    }
}

const char *BloomArea::getManagerName() const {
    return "ImageEffectArea";
}
 
#include "Game/AreaObj/SimpleBloomArea.hpp"
#include "Game/Util.hpp"

SimpleBloomArea::SimpleBloomArea(int type, const char *pName) :
    ImageEffectArea(IMAGE_EFFECT_TYPE_SIMPLE_BLOOM, type, pName) {
    _40 = 0;
    _44 = 128;
    _45 = 76;
}

SimpleBloomArea::~SimpleBloomArea() {
}

void SimpleBloomArea::init(const JMapInfoIter &rIter) {
    AreaObj::init(rIter);
    MR::createSimpleBloom();

    if (mObjArg0 >= 0) {
        _40 = static_cast<u8>(mObjArg0);
    }

    if (mObjArg1 >= 0) {
        _44 = static_cast<u8>(mObjArg1);
    }

    if (mObjArg2 >= 0) {
        _45 = static_cast<u8>(mObjArg2);
    }
}

const char *SimpleBloomArea::getManagerName() const {
    return "ImageEffectArea";
}

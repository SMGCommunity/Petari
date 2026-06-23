#include "Game/AreaObj/BloomArea.hpp"
#include "Game/Util/ScreenUtil.hpp"

BloomArea::BloomArea(int formType, const char* pName)
    : ImageEffectArea(IMAGE_EFFECT_TYPE_BLOOM, formType, pName), mThreshold(128), mIntensity(255), mIntensity1(-1), mIntensity2(-1) {
}

void BloomArea::init(const JMapInfoIter& rIter) {
    AreaObj::init(rIter);
    MR::createNormalBloom();

    if (mObjArg0 >= 0) {
        mIntensity = mObjArg0;
    }

    if (mObjArg1 >= 0) {
        mThreshold = mObjArg1;
    }

    if (mObjArg2 >= 0) {
        mIntensity1 = mObjArg2;
    }

    if (mObjArg3 >= 0) {
        mIntensity2 = mObjArg3;
    }
}

BloomArea::~BloomArea() {
}

#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class SimpleBloomArea : public ImageEffectArea {
public:
    enum EMaskFilterColor { MASK_FILTER_ALL, MASK_FILTER_RED, MASK_FILTER_GREEN, MASK_FILTER_BLUE };

    SimpleBloomArea(int, const char*);

    virtual ~SimpleBloomArea();
    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    /* 0x40 */ u32 mMaskFilterColor;
    /* 0x44 */ u8 mThreshold;
    /* 0x45 */ u8 mIntensity;
};

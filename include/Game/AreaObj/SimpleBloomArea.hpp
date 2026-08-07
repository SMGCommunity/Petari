#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class SimpleBloomArea : public ImageEffectArea {
public:
    enum EMaskFilterColor {
        /* 0x00 */ MASK_FILTER_ALL,
        /* 0x01 */ MASK_FILTER_RED,
        /* 0x02 */ MASK_FILTER_GREEN,
        /* 0x03 */ MASK_FILTER_BLUE,
    };

    SimpleBloomArea(int, const char*);

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    /* 0x40 */ u32 mMaskFilterColor;
    /* 0x44 */ u8 mThreshold;
    /* 0x45 */ u8 mIntensity;
};

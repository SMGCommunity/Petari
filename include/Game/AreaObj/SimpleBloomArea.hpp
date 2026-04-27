#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class SimpleBloomArea : public ImageEffectArea {
public:
    enum EMaskFilterColor { MASK_FILTER_ALL, MASK_FILTER_RED, MASK_FILTER_GREEN, MASK_FILTER_BLUE };

    SimpleBloomArea(int, const char*);
    virtual ~SimpleBloomArea();

    virtual void init(const JMapInfoIter&);

    virtual const char* getManagerName() const;

    u32 mMaskFilterColor;
    u8 _44;
    u8 _45;
    u8 _46[2];
};

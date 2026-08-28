#pragma once

#include "Game/AreaObj/ImageEffectArea.hpp"

class BloomArea : public ImageEffectArea {
public:
    BloomArea(int, const char*);

    /* 0x08 */ virtual ~BloomArea();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x28 */ virtual const char* getManagerName() const {
        return "ImageEffectArea";
    }

    /* 0x40 */ u8 mThreshold;
    /* 0x41 */ u8 mIntensity;
    /* 0x44 */ s32 mIntensity1;
    /* 0x48 */ s32 mIntensity2;
};

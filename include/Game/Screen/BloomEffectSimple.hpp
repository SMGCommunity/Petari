#pragma once

#include "Game/Screen/ImageEffectBase.hpp"

class JUTTexture;

class BloomEffectSimple : public ImageEffectBase {
public:
    /// @brief Creates a new `BloomEffectSimple`.
    BloomEffectSimple();

    /// @brief Draws the `BloomEffectSimple` to the screen.
    virtual void draw() const;

    /* 0x14 */ JUTTexture* _14;
    /* 0x18 */ JUTTexture* _18;
    /* 0x1C */ JUTTexture* _1C;
    /* 0x20 */ JUTTexture* _20;
    /* 0x24 */ JUTTexture* _24;
    /* 0x28 */ JUTTexture* _28;
    /* 0x2C */ JUTTexture* _2C;
    /* 0x30 */ s32 mMaskFilter;
    /* 0x34 */ u8 mThreshold;
    /* 0x38 */ f32 mIntensity;
};

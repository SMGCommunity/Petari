#pragma once

#include "Game/Screen/ImageEffectBase.hpp"

class JUTTexture;

class DepthOfFieldBlur : public ImageEffectBase {
public:
    /// @brief Creates a new `DepthOfFieldBlur`.
    /// @param pName A pointer to the null-terminated name of the object.
    DepthOfFieldBlur(const char* pName);

    /// @brief Draws the `DepthOfFieldBlur` to the screen.
    virtual void draw() const;

    void drawZAlphaTex() const;
    void createBlurTexture() const;
    void drawFinal() const;

    /* 0x14 */ JUTTexture* _14;
    /* 0x18 */ JUTTexture* _18;
    /* 0x1C */ JUTTexture* _1C;
    /* 0x20 */ JUTTexture* _20;
    /* 0x24 */ JUTTexture* _24;
    /* 0x28 */ JUTTexture* _28;
    /* 0x2C */ f32 mIntensity;
    /* 0x30 */ s32 mBlurMaxDist;
    /* 0x34 */ s32 mBlurMinDist;
};

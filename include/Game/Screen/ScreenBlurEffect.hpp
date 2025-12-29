#pragma once

#include "Game/Screen/ImageEffectBase.hpp"

class JUTTexture;

class ScreenBlurEffect : public ImageEffectBase {
public:
    /// @brief Creates a new `ScreenBlurEffect`.
    /// @param pName A pointer to the null-terminated name of the object.
    ScreenBlurEffect(const char* pName);

    /// @brief Draws the `ScreenBlurEffect` to the screen.
    virtual void draw() const;

    virtual void calcAnimSub();
    virtual void notifyTurnOn();

    void drawBlur() const;
    void captureBlur() const;

    /* 0x14 */ JUTTexture* _14;
    /* 0x18 */ JUTTexture* _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ bool _20;
    /* 0x24 */ f32 mIntensity;
};

#pragma once

#include <revolution/types.h>

class JUTTexture;

class ImageEffectResource {
public:
    /// @brief Creates a new `ImageEffectResource`.
    ImageEffectResource();

    void createBloomTexture();
    void createBlurTexture();
    void createDOFTexture();
    void createTexture(f32, GXTexFmt);

private:
    /* 0x00 */ JUTTexture* _0;
    /* 0x04 */ JUTTexture* _4;
    /* 0x08 */ JUTTexture* _8;
    /* 0x0C */ JUTTexture* _C;
    /* 0x10 */ JUTTexture* _10;
    /* 0x14 */ JUTTexture* _14;
    /* 0x18 */ JUTTexture* _18;
    /* 0x1C */ JUTTexture* _1C;
    /* 0x20 */ JUTTexture* _20;
};

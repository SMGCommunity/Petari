#pragma once

#include <revolution/types.h>

class JUTTexture;

class BigBubbleDrawer {
public:
    BigBubbleDrawer();

    void setUpDraw() const;
    void setUpVertexFormat() const;
    void setUpTexture() const;
    void setUpTevStage() const;
    void setUpPixelFormat() const;
    void setUpMaterial() const;

    JUTTexture* mBubbleTexture;   // 0x0
    JUTTexture* mRainbowTexture;  // 0x4
    f32 _8;
};

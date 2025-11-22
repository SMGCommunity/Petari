#pragma once

#include "Game/Util.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include <revolution.h>

namespace {
    static Color8 sTevColor0(0xE0E0E0FF);
    Color8 sTevColor1(0xFF000000);
    TVec3f sEnvMatTrans(1.0f, 1.0f, 1.5f);
};  // namespace

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
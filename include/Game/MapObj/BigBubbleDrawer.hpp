#pragma once

#include "JSystem/JUtility/JUTTexture.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>
#include "Game/Util.hpp"

namespace {
    static Color8 sTevColor0(0xE0E0E0FF);
    Color8 sTevColor1(0xFF000000);
    TVec3f sEnvMatTrans(1.0f, 1.0f, 1.5f);
};

class BigBubbleDrawer {
public:
    BigBubbleDrawer();

    void setUpDraw() const;
    void setUpVertexFormat() const;
    void setUpTexture() const;
    void setUpTevStage() const;
    void setUpPixelFormat() const;
    void setUpMaterial() const;

    JUTTexture* mBubbleTexture;     // _0
    JUTTexture* mRainbowTexture;    // _4
    f32 _8;
};
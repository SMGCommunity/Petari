#pragma once

#include <revolution/gx/GXEnum.h>

class BloomEffect;
class JUTTexture;
class NameObj;

namespace ImageEffectLocalUtil {
    enum ETexDrawType {
        TexDrawType_0,
        TexDrawType_1,
        TexDrawType_2,
        TexDrawType_3,
    };

    void capture(JUTTexture*, s32, s32, GXTexFmt, bool, u8);
    void setupDrawTexture();
    void drawTexture(JUTTexture*, s32, s32, u8, ETexDrawType);
    void sendTextureVertex(s32, s32);
    void blurTexture(JUTTexture*, s32, s32, u32, f32, f32);
};  // namespace ImageEffectLocalUtil

namespace MR {
    void connectToSceneImageEffect(NameObj*);
    void connectToSceneImageEffectMovement(NameObj*);
    void connectToSceneNormalBloom(BloomEffect*);
};  // namespace MR

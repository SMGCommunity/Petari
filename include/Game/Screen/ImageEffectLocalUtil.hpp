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

    void capture(JUTTexture* pTexture, s32 divisions, s32 tile, GXTexFmt format, bool clear, u8 filter);
    void setupDrawTexture();
    void drawTexture(JUTTexture* pTexture, s32 divisions, s32 tile, u8 intensity, ETexDrawType texDrawType);
    void sendTextureVertex(s32 divisions, s32 tile);
    void blurTexture(JUTTexture* pTexture, s32 divisions, s32 tile, u32 sampleCount, f32 radius, f32 intensity);
};  // namespace ImageEffectLocalUtil

namespace MR {
    void connectToSceneImageEffect(NameObj* pObj);
    void connectToSceneImageEffectMovement(NameObj* pObj);
    void connectToSceneNormalBloom(BloomEffect* pBloomEffect);
};  // namespace MR

#pragma once

#include <revolution/gx/GXEnum.h>

class JUTTexture;
class NameObj;

namespace ImageEffectLocalUtil {
    enum ETexDrawType { UNK_0, UNK_1, UNK_2 };

    void capture(JUTTexture*, s32, s32, GXTexFmt, bool, u8);
    void setupDrawTexture();
    void drawTexture(JUTTexture*, s32, s32, u8, ETexDrawType);
    void sendTextureVertex(s32, s32);
    void blurTexture(JUTTexture*, s32, s32, u32, f32, f32);
};  // namespace ImageEffectLocalUtil

namespace MR {
    void connectToSceneImageEffect(NameObj*);
    void connectToSceneImageEffectMovement(NameObj*);
};  // namespace MR

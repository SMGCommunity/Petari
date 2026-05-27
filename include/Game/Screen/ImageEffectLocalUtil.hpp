#pragma once

#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXEnum.h"
#include <revolution.h>

class NameObj;

namespace MR {
    void connectToSceneImageEffect(NameObj*);
    void connectToSceneImageEffectMovement(NameObj*);
};

namespace ImageEffectLocalUtil {
    enum ETexDrawType { UNK_0, UNK_1, UNK_2 };

    void setupDrawTexture();
    void drawTexture(JUTTexture*, s32, s32, u8, ETexDrawType);

    void blurTexture(JUTTexture*, s32, s32, u32, f32, f32);

    void capture(JUTTexture*, s32, s32, _GXTexFmt, bool, u8);
};  // namespace ImageEffectLocalUtil

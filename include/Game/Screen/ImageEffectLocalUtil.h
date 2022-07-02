#pragma once

#include <revolution.h>
#include "JSystem/JUtility/JUTTexture.h"

namespace ImageEffectLocalUtil {
    enum ETexDrawType {

    };

    void setupDrawTexture();
    void drawTexture(JUTTexture *, s32, s32, u8, ETexDrawType);
};
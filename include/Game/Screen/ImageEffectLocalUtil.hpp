#pragma once

#include "JSystem/JUtility/JUTTexture.hpp"
#include <revolution.h>

namespace ImageEffectLocalUtil {
    enum ETexDrawType {

    };

    void setupDrawTexture();
    void drawTexture(JUTTexture*, s32, s32, u8, ETexDrawType);
};  // namespace ImageEffectLocalUtil
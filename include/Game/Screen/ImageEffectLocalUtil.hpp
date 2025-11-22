#pragma once

#include <revolution.h>
#include "JSystem/JUtility/JUTTexture.hpp"

namespace ImageEffectLocalUtil {
    enum ETexDrawType {

    };

    void setupDrawTexture();
    void drawTexture(JUTTexture*, s32, s32, u8, ETexDrawType);
}; // namespace ImageEffectLocalUtil
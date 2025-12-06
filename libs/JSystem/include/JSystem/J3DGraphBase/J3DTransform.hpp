#pragma once

#include <revolution.h>

struct J3DTransformInfo {
    Vec mScale;        // 0x00
    S16Vec mRotation;  // 0x0C
    Vec mTranslate;    // 0x14
};

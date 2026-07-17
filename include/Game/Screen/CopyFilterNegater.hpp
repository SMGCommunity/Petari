#pragma once

#include <revolution/gx.h>

class CopyFilterNegater {
public:
    /// @brief Creates a new `CopyFilterNegater`.
    CopyFilterNegater();

    /// @brief Destroys the `CopyFilterNegater`.
    ~CopyFilterNegater();

    /* 0x00 */ GXRenderModeObj* mRenderObj;
};

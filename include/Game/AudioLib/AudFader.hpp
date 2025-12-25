#pragma once

#include <revolution/types.h>

class AudFader {
public:
    /// @brief Creates a new `AudFader`.
    AudFader();

    void set(f32, s32);
    void update();

    /* 0x00 */ f32 _0;
    /* 0x04 */ f32 _4;
    /* 0x08 */ f32 _8;
};

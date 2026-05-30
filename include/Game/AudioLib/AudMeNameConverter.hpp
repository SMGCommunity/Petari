#pragma once

#include <revolution/types.h>

class AudMeNameConverter {
public:
    /// @brief Creates a new `AudMeNameConverter`.
    AudMeNameConverter();

    void init();
    u32 getMeID(const char*) const;

    /* 0x00 */ u32 _0;
    /* 0x04 */ u32 _4;
};

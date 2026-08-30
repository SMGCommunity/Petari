#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include <revolution/types.h>

struct AudMeNameData {
    /* 0x0 */ const char* mName;
    /* 0x4 */ u32 mID;
    /* 0x8 */ u32 mHash;
};

class AudMeNameConverter {
public:
    /// @brief Creates a new `AudMeNameConverter`.
    AudMeNameConverter();

    void init();
    u32 getMeID(const char*) const;

    /* 0x00 */ u32 mNumEntries;
    /* 0x04 */ AudMeNameData* mEntries;
};

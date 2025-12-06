#pragma once

#include <revolution/types.h>

class PadRumbleInfo {
public:
    s32 mStartFrame;    // 0x0
    const char* mName;  // 0x4
};

class DemoPadRumbler {
public:
    DemoPadRumbler(const char*);
    void update(s32);

    s32 mNumPadRumbleEntries;          // 0x0
    PadRumbleInfo* mPadRumbleEntries;  // 0x4
    s32 _8;
};

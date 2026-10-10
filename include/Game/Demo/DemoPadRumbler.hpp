#pragma once

#include <revolution/types.h>

class PadRumbleInfo {
public:
    /* 0x0 */ s32 mStartFrame;
    /* 0x4 */ const char* mName;
};

class DemoPadRumbler {
public:
    DemoPadRumbler(const char*);
    void update(s32);

    /* 0x0 */ s32 mNumPadRumbleEntries;
    /* 0x4 */ PadRumbleInfo* mPadRumbleEntries;
    /* 0x8 */ s32 _8;
};

#pragma once

#include <revolution/types.h>

struct RumblePattern {
    /* 0x00 */ const char* mName;
    /* 0x04 */ s32 mFrame;
    /* 0x08 */ u8 mPattern[180];
    /* 0xBC */ u32 mHash;
};

namespace RumbleData {
    const RumblePattern* getPattern(const char*);
    void initHashValue();
    void checkHashCollision();
    u32 getTableSize();
    RumblePattern* getData(u16);
};  // namespace RumbleData

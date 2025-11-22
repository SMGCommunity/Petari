#pragma once

#include <revolution/types.h>

class HashSortTable {
public:
    HashSortTable(u32);

    bool add(const char*, u32, bool);
    bool add(u32, u32);
    bool addOrSkip(u32, u32);
    void sort();
    bool search(u32, u32*);
    bool search(const char*, u32*);
    bool search(const char*, const char*, u32*);
    void swap(const char*, const char*);

    /* 0x00 */ bool mHasBeenSorted;
    /* 0x04 */ u32* mHashCodes;
    /* 0x08 */ u32* _8;
    /* 0x0C */ u16* _C;
    /* 0x10 */ u16* _10;
    /* 0x14 */ u32 mCurrentLength;
    /* 0x18 */ u32 mMaxLength;
};

namespace MR {
    u32 getHashCode(const char*);
    u32 getHashCodeLower(const char*);
};  // namespace MR

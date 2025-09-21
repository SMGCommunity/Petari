#pragma once

#include <revolution.h>

class HashSortTable {
public:
    HashSortTable(u32);

    bool add(const char *, u32, bool);
    bool add(u32, u32);
    bool addOrSkip(u32, u32);
    void sort();
    s32 search(u32, u32 *);
    s32 search(const char *, u32 *);
    s32 search(const char *, const char *, u32 *);
    void swap(const char *, const char *);

    bool mHasBeenSorted; // 0x0
    u32* mHashCodes;   // 0x4
    u32* _8;
    u16* _C;
    u16* _10;
    u32 mCurrentLength; //0x14
    u32 mMaxLength; //0x18
};

namespace MR {
    u32 getHashCode(const char *);
    u32 getHashCodeLower(const char *);
};

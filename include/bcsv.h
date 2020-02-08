#ifndef BCSV_H
#define BCSV_H

#include <revolution.h>

struct BCSVEntry
{
    u32 mNameHash;
    u32 mMask;
    u16 mDataOffs;
    u8 mShift;
    u8 mField;
};

struct BCSVHeader
{
    u32 mEntryCount;
    u32 mFieldCount;
    u32 mDataOffs;
    u32 mEntrySize;
    BCSVEntry** mEntries;
};

#endif // BCSV_H
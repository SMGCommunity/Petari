#pragma once

#include "revolution.h"

struct SpkFile {
    u32 count;          // _0
    u32 entryOffset;    // _4
    u32 dataOffset;     // _8
    u32 _C;
};

struct SpkTableData {
    u32* dataArray;     // _0
};

class SpkTable {
public:
    SpkTable();

    void setResource(void *);

    bool mInitialized;      // _0
    u32 mResourceCount;     // _4
    u32 mEntryOffset;       // _8
    u32* mDataOffsets;        // _C
};
#pragma once

#include "revolution.h"

struct SpkFile {
    u32 count;        // 0x0
    u32 entryOffset;  // 0x4
    u32 dataOffset;   // 0x8
    u32 _C;
};

struct SpkTableData {
    u32* dataArray;  // 0x0
};

class SpkTable {
public:
    SpkTable();

    void setResource(void*);

    bool mInitialized;   // 0x0
    u32 mResourceCount;  // 0x4
    u32 mEntryOffset;    // 0x8
    u32* mDataOffsets;   // 0xC
};
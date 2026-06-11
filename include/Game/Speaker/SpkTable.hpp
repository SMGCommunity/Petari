#pragma once

#include "revolution.h"

struct SpkFile {
    /* 0x0 */ u32 count;
    /* 0x4 */ u32 parametersOffset;
    /* 0x8 */ u32 namesOffset;
    /* 0xC */ u32 initialized;
};

struct SpkTableData {
    /* 0x0 */ u32* dataArray;
};

class SpkTable {
public:
    SpkTable();

    void setResource(void*);

    /* 0x0 */ bool mInitialized;
    /* 0x4 */ s32 mResourceCount;
    /* 0x8 */ u32* mParameterOffsets;
    /* 0xC */ u32* mNameOffsets;
};

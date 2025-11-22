#pragma once

#include <revolution/types.h>

namespace MR {
    struct StationedFileInfo {
        /* 0x0 */ s32 mHeapType;
        /* 0x4 */ s32 mLoadType;
        /* 0x8 */ const char* mArchive;
    };

    const StationedFileInfo* getStationedFileInfoTable();
};  // namespace MR

#pragma once

#include <revolution.h>

namespace MR {
    struct StationedFileInfo {
        s32 mHeapType;              // 0x0
        s32 mLoadType;              // 0x4
        const char* mArchive;       // 0x8
    };

    const StationedFileInfo* getStationedFileInfoTable();
};

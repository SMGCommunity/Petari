#pragma once

#include <revolution.h>

namespace MR {
    struct StationedFileInfo {
        s32 mHeapType;              // _0
        s32 mLoadType;              // _4
        const char* mArchive;       // _8
    };

    const StationedFileInfo* getStationedFileInfoTable();
};

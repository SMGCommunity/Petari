#pragma once

#include <revolution/types.h>

namespace MR {
    struct StationedFileInfo {
        enum HeapType {
            HEAP_TYPE_NAPA = 0,
            HEAP_TYPE_GDDR = 1,
            HEAP_TYPE_NONE = 2,
        };

        enum LoadType {
            LOAD_TYPE_FILE = 0,
            LOAD_TYPE_MOUNT = 1,
            LOAD_TYPE_MOUNT_RESOURCE_MARIO = 2,
            LOAD_TYPE_MOUNT_RESOURCE_LUIGI = 3,
            LOAD_TYPE_MOUNT_RESOURCE = 4,
            LOAD_TYPE_MOUNT_RESOURCE_LAYOUT = 5,
        };

        /* 0x0 */ HeapType mHeapType;
        /* 0x4 */ LoadType mLoadType;
        /* 0x8 */ const char* mArchive;
    };

    const StationedFileInfo* getStationedFileInfoTable();
};  // namespace MR

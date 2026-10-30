#pragma once

#include <revolution/types.h>

namespace MR {
    struct StationedFileInfo {
        enum HeapType {
            /* 0x0 */ HEAP_TYPE_NAPA,
            /* 0x1 */ HEAP_TYPE_GDDR,
            /* 0x2 */ HEAP_TYPE_NONE,
        };

        enum LoadType {
            /* 0x0 */ LOAD_TYPE_FILE,
            /* 0x1 */ LOAD_TYPE_MOUNT,
            /* 0x2 */ LOAD_TYPE_MOUNT_RESOURCE_MARIO,
            /* 0x3 */ LOAD_TYPE_MOUNT_RESOURCE_LUIGI,
            /* 0x4 */ LOAD_TYPE_MOUNT_RESOURCE,
            /* 0x5 */ LOAD_TYPE_MOUNT_RESOURCE_LAYOUT,
        };

        /* 0x0 */ HeapType mHeapType;
        /* 0x4 */ LoadType mLoadType;
        /* 0x8 */ const char* mArchive;
    };

    const StationedFileInfo* getStationedFileInfoTable();
};  // namespace MR

#pragma once

#include <revolution.h>

class HeapMemoryWatcher {
public:
    HeapMemoryWatcher();

    void setCurrentHeapToStationedHeap();

    static void createRootHeap();

    u8 _0[0x28];
};
#pragma once

#include <revolution/os.h>

class GameSystemException {
public:
    static void init();
    static void handleException(OSError, OSContext*, u32, u32);

    static void* sMapFileUsingBuffer;
};

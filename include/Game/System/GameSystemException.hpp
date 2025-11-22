#pragma once

#include <revolution.h>

class GameSystemException {
public:
    static void init();
    static void handleException(u16, OSContext*, u32, u32);

    static void* sMapFileUsingBuffer;
};

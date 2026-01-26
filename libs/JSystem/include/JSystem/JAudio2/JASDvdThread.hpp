#pragma once

#include <revolution/types.h>

class JASTaskThread;

class JASDvd {
public:
    static JASTaskThread* getThreadPointer();
    static bool createThread(s32 priority, int msgCount, u32 stackSize);

    static JASTaskThread* sThread;
};

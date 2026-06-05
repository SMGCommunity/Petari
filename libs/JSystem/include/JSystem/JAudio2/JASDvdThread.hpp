#pragma once

#include <revolution/types.h>

class JASTaskThread;

namespace JASDvd {
    JASTaskThread* getThreadPointer();
    bool createThread(s32 priority, int msgCount, u32 stackSize);
};  // namespace JASDvd

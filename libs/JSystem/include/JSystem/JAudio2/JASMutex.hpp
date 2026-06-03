#pragma once

#include <revolution/os/OSMutex.h>

class JASMutexLock {
public:
    inline JASMutexLock(OSMutex* mutex) {
        mMutex = mutex;
        OSLockMutex(mMutex);
    }
    inline ~JASMutexLock() {
        OSUnlockMutex(mMutex);
    }

    /* 0x0 */ OSMutex* mMutex;
};

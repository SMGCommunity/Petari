#pragma once

#include <revolution/os/OSMutex.h>

class JASMutexLock {
public:
    JASMutexLock(OSMutex* mutex) {
        mMutex = mutex;
        OSLockMutex(mMutex);
    }
    ~JASMutexLock() { OSUnlockMutex(mMutex); }

    /* 0x0 */ OSMutex* mMutex;
};

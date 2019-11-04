#ifndef OSMUTEX_H
#define OSMUTEX_H

#include "OS/OSThread.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OSMutex
{
    OSThreadQueue mQueue;
    OSThread* mThread;
    s32 mLockCount;
    OSMutexLink mLink;
};

void OSLockMutex(OSMutex *);
void OSUnlockMutex(OSMutex *);

#ifdef __cplusplus
}
#endif

#endif // OSMUTEX_H
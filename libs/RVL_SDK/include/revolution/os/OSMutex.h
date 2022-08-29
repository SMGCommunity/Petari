#ifndef OSMUTEX_H
#define OSMUTEX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <revolution/os/OSThread.h>

struct OSMutex {
    OSThreadQueue queue;
    OSThread* thread;
    s32 count;
    OSMutexLink link;
};

struct OSCond {
    OSThreadQueue queue;
};

void OSInitMutex(OSMutex *);
void OSLockMutex(OSMutex *);
void OSUnlockMutex(OSMutex *);
BOOL OSTryLockMutex(OSMutex *);

#ifdef __cplusplus
}
#endif

#endif // OSMUTEX_H
#ifndef OSTHREAD_H
#define OSTHREAD_H

#include <revolution/os/OSContext.h>
#include <revolution/os/OSTime.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread OSThread;
typedef struct OSThreadQueue OSThreadQueue;
typedef struct OSThreadLink OSThreadLink;
typedef s32 OSPriority;

typedef struct OSMutex OSMutex;
typedef struct OSMutexQueue OSMutexQueue;
typedef struct OSMutexLink OSMutexLink;
typedef struct OSCond OSCond;

typedef void (*OSSwitchThreadCallback)(OSThread *, OSThread *);

struct OSThreadQueue {
    OSThread* head;
    OSThread* tail;
};

struct OSThreadLink {
    OSThread* next;
    OSThread* prev;
};

struct OSMutexQueue {
    OSMutex* head;
    OSMutex* tail;
};

struct OSMutexLink {
    OSMutex* next;
    OSMutex* prev;
};

struct OSThread {
    OSContext context;
    u16 state;
    u16 attr;
    s32 suspend;
    OSPriority priority;
    OSPriority base;
    void* value;
    OSThreadQueue* queue;
    OSThreadLink link;
    OSThreadQueue queueJoin;
    OSMutex* mutex;
    OSMutexQueue queueMutex;
    OSThreadLink linkActive;
    u8* stackBase;
    u32* stackEnd;
    s32 error;
    void* specific[2];
};

enum OS_THREAD_STATE {
    OS_THREAD_STATE_READY    =  1,
    OS_THREAD_STATE_RUNNING  =  2,
    OS_THREAD_STATE_WAITING  =  4,
    OS_THREAD_STATE_MORIBUND =  8
};

void __OSThreadInit(void);
void OSInitThreadQueue(OSThreadQueue *);
OSThread* OSGetCurrentThread(void);
BOOL OSIsThreadSuspended(OSThread *);
BOOL OSIsThreadTerminated(OSThread *);
s32 OSDisableScheduler(void);
s32 OSEnableScheduler(void);

void OSYieldThread(void);
BOOL OSCreateThread(OSThread *, void* (*func)(void*), void *, void *, u32, OSPriority, u16);
void OSExitThread(void *);
void OSCancelThread(OSThread *);
BOOL OSJoinThread(OSThread *, void **);
void OSDetachThread(OSThread *);
s32 OSResumeThread(OSThread *);
s32 OSSuspendThread(OSThread *);
void OSSleepThread(OSThreadQueue *);
void OSWakeupThread(OSThreadQueue *);
BOOL OSSetThreadPriority(OSThread *, OSPriority);
OSPriority OSGetThreadPriority(OSThread *);

void OSSleepTicks(OSTime);

#ifdef __cplusplus
}
#endif

#endif // OSTHREAD_H

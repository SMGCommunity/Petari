#ifndef OSTHREAD_H
#define OSTHREAD_H

#include "types.h"
#include "OS/OSContext.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OSThread         OSThread;
typedef struct OSThreadQueue    OSThreadQueue;
typedef struct OSThreadLink     OSThreadLink;

typedef struct OSMutex          OSMutex;
typedef struct OSMutexQueue     OSMutexQueue;
typedef struct OSMutexLink      OSMutexLink;
typedef struct OSCond           OSCond;

// thread priority, 0 being highest, and 31 being lowest
typedef s32 OSPriority;

static s32 Reschedule;

struct OSThreadQueue
{
    OSThread* mHead; // _0
    OSThread* mTail; // _4
};

struct OSThreadLink
{
    OSThread* mNext; // _0
    OSThread* mPrev; // _4
};

struct OSMutexQueue
{
    OSMutex* mHead; // _0
    OSMutex* mTail; // _4
};

struct OSMutexLink
{
    OSMutex* mNext; // _0
    OSMutex* mPrev; // _4
};

struct OSThread
{
    OSContext mContext;
    u16 mState;
    u16 mAttr;
    s32 mSuspend;
    OSPriority mPriority;
    OSPriority mBase; // _2D4
    void* mVal; // _2D8
    OSThreadQueue* mQueue; // _2DC
    OSThreadLink mLink; // _2E0
    OSThreadQueue mQueueJoin; // _2E8
    OSMutex* mMutex; // _2F0
    OSMutexQueue mQueueMutex; // _2F4
    OSThreadLink mLinkActive;
    u8* mStackBase;
    u32* mStackEnd;
    s32 mError;
    void* mSpecific[2];
};

void OSInitThreadQueue(OSThreadQueue *);
OSThread* OSGetCurrentThread();
u32 OSIsThreadSuspended(OSThread *);
u32 OSIsThreadTerminated(OSThread *);
s32 OSDisableScheduler();
s32 OSEnableScheduler();

s32 __OSGetEffectivePriority(OSThread *);

void OSYieldThread();

void OSCancelThread(OSThread *);
void OSDetachThread(OSThread *);

s32 OSCreateThread(OSThread *thread, void* (*startFunc)(void *), void *param, void *stack, u32 stackSize, OSPriority priority, u16 attribute);

#ifdef __cplusplus
}
#endif

#endif // OSTHREAD_H
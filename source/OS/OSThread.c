#include "OS/OSThread.h"

// slightly inaccurate
s32 OSIsThreadSuspended(OSThread* thread)
{
    s32 suspendState = thread->mSuspend;
    return (-(suspendState) & (suspendState)) >> 31;
}

u32 OSIsThreadTerminated(OSThread* thread)
{
    u32 ret = 1;

    if (thread->mState != 8 && thread->mState != 0)
    {
        ret = 0;
    }

    return  (u32)((-ret | ret) >> 31);
}

/*s32 OSDisableScheduler()
{
    OSDisableInterrupts();
    s32 scheduler = Reschedule;
    OSRestoreInterrupts();
    return scheduler;
}*/

void OSInitThreadQueue(OSThreadQueue* threadQueue)
{
    threadQueue->mHead = 0;
    threadQueue->mTail = 0;
}

/*s32 __OSGetEffectivePriority(OSThread* thread)
{
    s32 base = thread->mBase;

    if (thread->mQueueMutex.mHead == 0)
    {
        return base;
    }

    return base;
}*/
#include "System/OS/OSThreadWrapper.h"

OSThreadWrapper::OSThreadWrapper(u32 stackSize, s32 msgCount, s32 priority, JKRHeap* heap)
{
    if (heap == 0)
    {
        heap = JKRHeap::sCurrentHeap;
    }

    initHeapSpecified(heap, stackSize, priority);
    initMessageQueue(heap, msgCount);
}

void OSThreadWrapper::initMessageQueue(JKRHeap* heap, s32 msgCount)
{
    /*mMessageCount = msgCount;
    mMessages = new(heap, 0) OSMessage[msgCount];
    OSInitMessageQueue(&mQueue, mMessages, mMessageCount);*/
}

void OSThreadWrapper::initHeapSpecified(JKRHeap* heap, u32 stackSize, s32 priority)
{
    // todo
}

s32 OSThreadWrapper::start(void *src)
{
    return run();
}

s32 OSThreadWrapper::run()
{
    return 0;
}
#include "JKernel/JKRThread.h"
#include "JKernel/JKRHeap.h"

JKRThread::JKRThread(u32 stackSize, s32 messageCount, s32 threadPriority) : _18(this), JKRDisposer()
{
    TLoad mLoad();

    JKRHeap* heap = JKRHeap::findFromRoot(this);

    if (!heap)
    {
        heap = JKRHeap::sSystemHeap;
    }

    setCommon_heapSpecified(heap, stackSize, threadPriority);
    setCommon_mesgQueue(_28, messageCount);
}

JKRThread::JKRThread(JKRHeap *pHeap, u32 stackSize, s32 messageCount, s32 threadPriority) : _18(this), JKRDisposer()
{
    TLoad mLoad();

    if (!pHeap)
    {
        pHeap = JKRHeap::sSystemHeap;
    }

    setCommon_heapSpecified(pHeap, stackSize, threadPriority);
    setCommon_mesgQueue(_28, messageCount);
}

void JKRThread::setCommon_mesgQueue(JKRHeap *pHeap, s32 msgCount)
{
    mMessageCount = msgCount;
    mMessage = (OSMessage*)JKRHeap::alloc(msgCount * 4, 0, pHeap);
    OSInitMessageQueue(&mQueue, mMessage, mMessageCount);
    sThreadList->append(&_18);
    _74 = 0;
    _78 = 0;
}

void JKRThread::setCommon_heapSpecified(JKRHeap *pHeap, u32 stackSize, u32 threadPriority)
{
    // todo
}

void JKRThread::start(void *src)
{
    run();
}

JKRThread::TLoad::TLoad()
{
    clear();
    _0 = 0;
    _10 = 0;
}

void JKRThread::TLoad::clear()
{
    _8 = 0;
    _4 = 0;
    _C = 0;
}
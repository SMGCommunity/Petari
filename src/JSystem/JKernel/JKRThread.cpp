#include "JSystem/JKernel/JKRThread.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

JSUList< JKRThread > JKRThread::sThreadList = JSUList< JKRThread >(false);
JSUList< JKRTask > JKRThread::sTaskList = JSUList< JKRTask >();

JKRThread::JKRThread(u32 stack_size, int message_count, int param_3) : mThreadListLink(this) {
    JKRHeap* heap = JKRHeap::findFromRoot(this);
    if (heap == NULL) {
        heap = JKRGetSystemHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_3);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(JKRHeap* heap, u32 stack_size, int message_count, int param_4) : mThreadListLink(this) {
    if (heap == NULL) {
        heap = JKRGetCurrentHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_4);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::TLoad::TLoad() {
    clear();

    _0 = 0;
    _10 = 0;
}

void JKRThread::TLoad::clear() {
    _8 = 0;
    _4 = 0;
    _C = 0;
}

JKRThread::~JKRThread() {
    sThreadList.remove(&mThreadListLink);

    if (mHeap) {
        if (!OSIsThreadTerminated(mThread)) {
            OSDetachThread(mThread);
            OSCancelThread(mThread);
        }

        JKRHeap::free(mStackMemory, mHeap);
        JKRHeap::free(mThread, mHeap);
    }

    JKRHeap::free(mMesgBuffer, 0);
}

void JKRThread::setCommon_mesgQueue(JKRHeap* pHeap, int msgCount) {
    mMessageCount = msgCount;
    mMesgBuffer = reinterpret_cast< OSMessage* >(JKRHeap::alloc(mMessageCount * 4, 0, pHeap));
    OSInitMessageQueue(&mMessageQueue, mMesgBuffer, mMessageCount);
    sThreadList.append(&mThreadListLink);
    mCurrentHeap = nullptr;
    mCurrentHeapError = 0;
}

void JKRThread::setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int param_3) {
    mHeap = heap;
    mStackSize = stack_size & 0xffffffe0;
    mStackMemory = JKRAllocFromHeap(mHeap, mStackSize, 0x20);
    mThread = (OSThread*)JKRAllocFromHeap(mHeap, sizeof(OSThread), 0x20);
    OSCreateThread(mThread, start, this, (u8*)mStackMemory + mStackSize, mStackSize, param_3, 1);
}

void* JKRThread::start(void* thread) {
    return ((JKRThread*)thread)->run();
}

JKRThread* JKRThread::searchThread(OSThread* pThread) {
    for (JSUPtrLink* i = sThreadList.mHead; i; i = i->mNext) {
        JKRThread* thread = reinterpret_cast< JKRThread* >(i->mData);

        if (thread->mThread == pThread) {
            return thread;
        }
    }

    return 0;
}

JKRThread* JKRThreadSwitch::enter(JKRThread* pThread, int a2) {
    JKRThread* thread = pThread;

    if (!pThread) {
        return 0;
    }

    JKRThread* foundThread = JKRThread::searchThread(pThread->mThread);

    if (foundThread) {
        thread = foundThread;
    }

    JKRThread::TLoad* inf = &thread->mLoadInfo;

    inf->clear();
    inf->_10 = a2;
    inf->_0 = 1;

    return thread;
}

void* JKRThread::run() {
    return nullptr;
}

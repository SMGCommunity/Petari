#include "JSystem/JKernel/JKRThread.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

JSUList< JKRThread > JKRThread::sThreadList = JSUList< JKRThread >(false);
JSUList< JKRTask > JKRThread::sTaskList = JSUList< JKRTask >();

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

/*
void JKRThread::setCommon_heapSpecified(JKRHeap *pHeap, u32 a2, int a3) {
    mHeap = pHeap;
    _5C = (void*)(a3 & 0xFFFFFFE0);
    mStackMemory = JKRHeap::alloc(*(u32*)_5C, 32, pHeap);
    OSThread* thread = reinterpret_cast<OSThread*>(JKRHeap::alloc(0x318, 0x20, mHeap));
    mThreadRecord = thread;
    OSCreateThread(thread, JKRThread::start, this, 0, 0, 0, 0);
    //OSCreateThread(thread, JKRThread::start, this, (u8*)_5C + mStackMemory, _5C, a3, 1);
}
*/

void* JKRThread::start(void* pData) {
    return run();
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

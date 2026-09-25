#include "JSystem/JKernel/JKRThread.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"
#include "JSystem/JUtility/TColor.hpp"

JKRThread::JKRThread(u32 stack_size, int message_count, int param_3) : mThreadListLink(this) {
    JKRHeap* heap = JKRHeap::findFromRoot(this);
    if (heap == nullptr) {
        heap = JKRGetSystemHeap();
    }

    setCommon_heapSpecified(heap, stack_size, param_3);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::JKRThread(JKRHeap* pHeap, u32 stack_size, int message_count, int param_4) : mThreadListLink(this) {
    if (pHeap == nullptr) {
        pHeap = JKRGetCurrentHeap();
    }

    setCommon_heapSpecified(pHeap, stack_size, param_4);
    setCommon_mesgQueue(mHeap, message_count);
}

JKRThread::~JKRThread() {
    sThreadList.remove(&mThreadListLink);

    if (mHeap != nullptr) {
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

void JKRThread::setCommon_heapSpecified(JKRHeap* pHeap, u32 stack_size, int param_3) {
    mHeap = pHeap;
    mStackSize = stack_size & 0xffffffe0;
    mStackMemory = JKRAllocFromHeap(mHeap, mStackSize, 0x20);
    mThread = reinterpret_cast< OSThread* >(JKRAllocFromHeap(mHeap, sizeof(OSThread), 0x20));
    OSCreateThread(mThread, start, this, reinterpret_cast< u8* >(mStackMemory) + mStackSize, mStackSize, param_3, 1);
}

void* JKRThread::start(void* pThread) {
    return static_cast< JKRThread* >(pThread)->run();
}

JKRThread* JKRThread::searchThread(OSThread* pThread) {
    for (JSUPtrLink* i = sThreadList.mHead; i != nullptr; i = i->mNext) {
        JKRThread* thread = reinterpret_cast< JKRThread* >(i->mData);

        if (thread->mThread == pThread) {
            return thread;
        }
    }

    return nullptr;
}

JKRThread* JKRThreadSwitch::enter(JKRThread* pThread, int a2) {
    if (pThread == nullptr) {
        return nullptr;
    }

    JKRThread* foundThread = JKRThread::searchThread(pThread->mThread);

    if (foundThread != nullptr) {
        pThread = foundThread;
    }

    JKRThread::TLoad* inf = &pThread->mLoadInfo;

    inf->clear();
    inf->_10 = a2;
    inf->_0 = 1;

    return pThread;
}

JUtility::TColor& JUtility::TColor::operator=(const TColor& rColor) {
    static_cast< GXColor& >(*this) = rColor;
    return *this;
}

void setThreadColor(JUtility::TColor& rDestination, u8 r, u8 g, u8 b, u8 a) {
    rDestination = JUtility::TColor(r, g, b, a);
}

JSUList< JKRThread > JKRThread::sThreadList = JSUList< JKRThread >(false);
u64 JKRThreadSwitch::sTotalStart;
JSUList< JKRTask > JKRTask::sTaskList = JSUList< JKRTask >();
u8 JKRTask::sEndMesgQueue[32];
JKRThreadSwitch* JKRThreadSwitch::sManager;
u32 JKRThreadSwitch::sTotalCount;

void JKRThread_FORCE_MATCH(JUtility::TColor* pColor, u8 r, u8 g, u8 b, u8 a) {
    *pColor = JUtility::TColor(r, g, b, a);
    *pColor = JUtility::TColor(r, g, b, a);
    *pColor = JUtility::TColor(r, g, b, a);
    *pColor = JUtility::TColor(r, g, b, a);
}

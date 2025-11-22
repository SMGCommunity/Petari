#include "Game/System/OSThreadWrapper.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

OSThreadWrapper::OSThreadWrapper(u32 stackSize, int messageCount, int priority, JKRHeap* pHeap) {
    if (pHeap == nullptr) {
        pHeap = JKRHeap::sCurrentHeap;
    }

    initHeapSpecified(pHeap, stackSize, priority);
    initMessageQueue(mHeap, messageCount);
}

OSThreadWrapper::~OSThreadWrapper() {
    if (mHeap != nullptr) {
        if (!OSIsThreadTerminated(mThread)) {
            OSDetachThread(mThread);
            OSCancelThread(mThread);
        }

        delete[] mStack;
        delete mThread;
    }

    delete[] mMessageArray;
}

void OSThreadWrapper::initMessageQueue(JKRHeap* pHeap, int messageCount) {
    mMessageCount = messageCount;
    mMessageArray = new (pHeap, 0) OSMessage[mMessageCount];

    OSInitMessageQueue(&mQueue, mMessageArray, mMessageCount);
}

void OSThreadWrapper::initHeapSpecified(JKRHeap* pHeap, u32 stackSize, int priority) {
    mHeap = pHeap;
    mStackSize = stackSize & ~31;
    mStack = new (mHeap, 32) u8[mStackSize];
    mThread = new (mHeap, 32) OSThread();

    OSCreateThread(
        mThread,
        &OSThreadWrapper::start,
        this,
        &mStack[mStackSize],
        mStackSize,
        priority,
        1 /* OS_THREAD_ATTR_DETACH */);
}

void* OSThreadWrapper::start(void* pArg) {
    return static_cast<OSThreadWrapper*>(pArg)->run();
}

void* OSThreadWrapper::run() {
    return nullptr;
}

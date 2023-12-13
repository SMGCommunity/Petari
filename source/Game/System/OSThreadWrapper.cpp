#include "Game/System/OSThreadWrapper.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

OSThreadWrapper::OSThreadWrapper(u32 a1, int a2, int a3, JKRHeap *pHeap) {
    if (!pHeap) {
        pHeap = JKRHeap::sCurrentHeap;
    }

    initHeapSpecified(pHeap, a1, a3);
    initMessageQueue(_4, a2);
}

OSThreadWrapper::~OSThreadWrapper() {
    if (_4) {
        if (!OSIsThreadTerminated(mThread)) {
            OSDetachThread(mThread);
            OSCancelThread(mThread);
        }

        delete[] _34;
        delete mThread;
    }

    delete[] mMessageArray;
}

void OSThreadWrapper::initMessageQueue(JKRHeap *pHeap, int messageCount) {
    mMessageCount = messageCount;
    mMessageArray = new(pHeap, 0) OSMessage[mMessageCount];
    OSInitMessageQueue(&mQueue, mMessageArray, mMessageCount);
}

/*
void OSThreadWrapper::initHeapSpecified(JKRHeap *pHeap, u32 a2, int priority) {
    _4 = pHeap;
    _38 = a2 & 0xFFFFFFE0;
    _34 = new(pHeap, 32) u8[_38];
    OSThread* thread = new(pHeap, 32) OSThread();
    mThread = thread;
    OSCreateThread(mThread, OSThreadWrapper::start, this, &_34[_38], _38, priority, 1);
}
*/

s32 OSThreadWrapper::start(void * /* unused */) {
    return run();
}

s32 OSThreadWrapper::run() {
    return 0;
} 
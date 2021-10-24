#include "Game/System/OSThreadWrapper.h"
#include "JSystem/JKernel/JKRHeap.h"

OSThreadWrapper::OSThreadWrapper(u32 a1, int a2, int a3, JKRHeap *pHeap) {
    if (!pHeap) {
        pHeap = JKRHeap::sCurrentHeap;
    }

    initHeapSpecified(pHeap, a1, a3);
    initMessageQueue(_4, a2);
}

// OSThreadWrapper::~OSThreadWrapper
// OSThreadWrapper::initMessageQueue
// OSThreadWrapper::initHeapSpecified

s32 OSThreadWrapper::start(void * /* unused */) {
    return run();
}

s32 OSThreadWrapper::run() {
    return 0;
}
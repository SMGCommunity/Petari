#include "Game/System/FunctionAsyncExecutor.h"
#include <revolution.h>

FunctionAsyncExecInfo::FunctionAsyncExecInfo(MR::FunctorBase *pFuncPtr, int a2, const char *a3) {
    mFunc = pFuncPtr;
    mPriority = a2;
    _8 = a3;
    _C = 0;
    OSInitMessageQueue(&mQueue, &mMessage, 1);
}

FunctionAsyncExecInfo::~FunctionAsyncExecInfo() {
    delete mFunc;
}

void FunctionAsyncExecInfo::execute() {
    OSThread* thread = OSGetCurrentThread();
    OSPriority prio = OSGetThreadPriority(thread);
    OSSetThreadPriority(thread, mPriority);
    (*mFunc)();
    OSSetThreadPriority(thread, prio);
}

FunctionAsyncExecutorThread::FunctionAsyncExecutorThread(JKRHeap *pHeap) : OSThreadWrapper(0x10000, 4, 1, pHeap) {
    _3C = 0;
    _40 = 0;
}

s32 FunctionAsyncExecutorThread::run() {
    __asm {
        li r3, 4
        oris r3, r3, 4
        mtspr 0x392, r3
        li r3, 5
        oris r3, r3, 5
        mtspr 0x393, r3
        li r3, 6
        oris r3, r3, 6
        mtspr 0x394, r3
        li r3, 7
        oris r3, r3, 7
        mtspr 0x395, r3
    };

    while(1) {
        _3C = 0;
        _40 = 0;
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, OS_MESSAGE_BLOCK);
        FunctionAsyncExecInfo* info = reinterpret_cast<FunctionAsyncExecInfo*>(msg);
        _40 = info->_8;
        _3C = 1;
        info->execute();
        OSSendMessage(&info->mQueue, 0, 0);
        info->_C = 1;
    }
}
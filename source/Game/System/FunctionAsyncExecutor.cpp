#include "Game/System/FunctionAsyncExecutor.hpp"
#include <JSystem/JKernel/JKRUnitHeap.hpp>
#include "Game/Util.hpp"
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
    mIsSuspended = false;
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
        mIsSuspended = false;
        _40 = 0;
        OSMessage msg;
        OSReceiveMessage(&mQueue, &msg, OS_MESSAGE_BLOCK);
        FunctionAsyncExecInfo* info = reinterpret_cast<FunctionAsyncExecInfo*>(msg);
        _40 = info->_8;
        mIsSuspended = true;
        info->execute();
        OSSendMessage(&info->mQueue, 0, 0);
        info->_C = 1;
    }
}

FunctionAsyncExecutorOnMainThread::FunctionAsyncExecutorOnMainThread(OSThread* pThread) {
    mThread = pThread;
    _0 = 0;
    OSInitMessageQueue(&mQueue, mMsgArray, 0x40);
}

void FunctionAsyncExecutorOnMainThread::update() {
    OSMessage msg;
    FunctionAsyncExecInfo* info;
    if (OSReceiveMessage(&mQueue, &msg, 0)) {
        _0 = 1;
        info = (FunctionAsyncExecInfo*)msg;
        info->mPriority = OSGetThreadPriority(OSGetCurrentThread());
        info->execute();
        OSSendMessage(&info->mQueue, 0, 0);
        info->_C = 1;
    }
}

FunctionAsyncExecutor::FunctionAsyncExecutor() {
    mMainThreadExec = nullptr;
    _40C = 0;
    _410 = nullptr;
    _414 = nullptr;

    for (int i = 0; i < 2; i++) {
        FunctionAsyncExecutorThread* thread = new FunctionAsyncExecutorThread(MR::getStationedHeapNapa());
        mThreads[i] = thread;
        OSResumeThread(thread->mThread);
    }

    mMainThreadExec = new FunctionAsyncExecutorOnMainThread(OSGetCurrentThread());
    _410 = JKRUnitHeap::create(0x34, 0x34A8, 4, MR::getCurrentHeap(), false);
    _414 = JKRExpHeap::create(0x2800, MR::getCurrentHeap(), false);
}

void FunctionAsyncExecutor::update() {
    mMainThreadExec->update();
}

void FunctionAsyncExecutor::start(const MR::FunctorBase &rBase, int priority, const char *pName) {
    FunctionAsyncExecInfo* info = createAndAddExecInfo(rBase, priority, pName);
    FunctionAsyncExecutorThread* thread = getSuspendThread();
    OSSendMessage(&thread->mQueue, info, 0);
}

bool FunctionAsyncExecutor::startOnMainThread(const MR::FunctorBase &rBase, const char *pName) {
    FunctionAsyncExecutorOnMainThread* thread = mMainThreadExec;
    bool isSameThread = OSGetCurrentThread() == thread->mThread;

    if (isSameThread) {
        rBase();
        return false;
    }
    else {
        FunctionAsyncExecInfo* info = createAndAddExecInfo(rBase, 0, pName);
        OSSendMessage(&mMainThreadExec->mQueue, info, 0);
        return true;
    }
}

void FunctionAsyncExecutor::waitForEnd(const char *pName) {
    OSLockMutex(&MR::MutexHolder<2>::sMutex);
    FunctionAsyncExecInfo** cur = (FunctionAsyncExecInfo**)first();
    FunctionAsyncExecInfo* const* lst = last();

    while (cur != lst && (*cur)->isSame(pName)) {
        cur++;
    }

    FunctionAsyncExecInfo* info = *cur;
    OSUnlockMutex(&MR::MutexHolder<2>::sMutex);
    OSReceiveMessage(&info->mQueue, &info->mMessage, OS_MESSAGE_BLOCK);
    OSLockMutex(&MR::MutexHolder<2>::sMutex);
    cur = (FunctionAsyncExecInfo**)first();
    lst = last();

    while (cur != lst && *cur != info) {
        cur++;
    }

    if (lst - cur - 1 > 0) {
        while (cur + 1 != last()) {
            *cur = cur[1];
            cur++;
        }
    }

    _40C--;
    OSUnlockMutex(&MR::MutexHolder<2>::sMutex);
    delete info;
}

/* this matches but on a different compiler version */
bool FunctionAsyncExecutor::isEnd(const char *pName) const {
    OSLockMutex(&MR::MutexHolder<2>::sMutex);
    FunctionAsyncExecInfo* const* cur = first();
    FunctionAsyncExecInfo* const* lst = last();

    while((cur != lst) && (*cur)->isSame(pName)) {
        cur++;
    } 
 
    FunctionAsyncExecInfo* info = *cur;
    OSUnlockMutex(&MR::MutexHolder<2>::sMutex);
    return info->_C;
}

OSThread* FunctionAsyncExecutor::getOSThread(const char *pName) {
    for (int i = 0; i < 2; i++) {
        FunctionAsyncExecutorThread* thread = mThreads[i];

        if (thread->mIsSuspended && MR::isEqualString(thread->_40, pName)) {
            return thread->mThread;
        }
    }

    return nullptr;
}

FunctionAsyncExecInfo* FunctionAsyncExecutor::createAndAddExecInfo(const MR::FunctorBase &rBase, int priority, const char *pName) {
    MR::FunctorBase* func = rBase.clone(_414);
    FunctionAsyncExecInfo* info = new(_410, 0) FunctionAsyncExecInfo(func, priority, pName);

    OSLockMutex(&MR::MutexHolder<2>::sMutex);
    s32 cnt = _40C;
    _40C = cnt + 1;
    mHolders[cnt] = info;
    OSUnlockMutex(&MR::MutexHolder<2>::sMutex);
    return info;
}

FunctionAsyncExecutorThread* FunctionAsyncExecutor::getSuspendThread() {
    for (int i = 0; i < 2; i++) {
        if (mThreads[i]->mIsSuspended) {
            return mThreads[i];
        }
    }

    return nullptr;
}

FunctionAsyncExecutorThread::~FunctionAsyncExecutorThread() {

}

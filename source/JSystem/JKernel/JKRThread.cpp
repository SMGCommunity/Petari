#include "JSystem/JKernel/JKRThread.h"
#include "JSystem/JKernel/JKRHeap.h"

JSUList<JKRThread> JKRThread::sThreadList = JSUList<JKRThread>(false);
JSUList<JKRTask> JKRThread::sTaskList = JSUList<JKRTask>();

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
    sThreadList.remove(&mDisposerList);
    
    if (_28) {
        if (!OSIsThreadTerminated(mThread)) {
            OSDetachThread(mThread);
            OSCancelThread(mThread);
        }

        JKRHeap::free(_58, _28);
        JKRHeap::free(mThread, _28);
    }

    JKRHeap::free(mMessage, 0);
}

void JKRThread::setCommon_mesgQueue(JKRHeap *pHeap, int msgCount) {
    mMsgCount = msgCount;
    mMessage = reinterpret_cast<OSMessage*>(JKRHeap::alloc(mMsgCount * 4, 0, pHeap));
    OSInitMessageQueue(&mQueue, mMessage, mMsgCount);
    sThreadList.append(&mDisposerList);
    _74 = 0;
    _78 = 0;
}

/*
void JKRThread::setCommon_heapSpecified(JKRHeap *pHeap, u32 a2, int a3) {
    _28 = pHeap;
    _5C = (void*)(a3 & 0xFFFFFFE0);
    _58 = JKRHeap::alloc(*(u32*)_5C, 32, pHeap);
    OSThread* thread = reinterpret_cast<OSThread*>(JKRHeap::alloc(0x318, 0x20, _28));
    mThread = thread;
    OSCreateThread(thread, JKRThread::start, this, 0, 0, 0, 0);
    //OSCreateThread(thread, JKRThread::start, this, (u8*)_5C + _58, _5C, a3, 1);
}
*/

s32 JKRThread::start(void *pData) {
    return run();
}

JKRThread* JKRThread::searchThread(OSThread *pThread) {
    for (JSUPtrLink* i = sThreadList.mHead; i; i = i->mNext) {
        JKRThread* thread = reinterpret_cast<JKRThread*>(i->mData);

        if (thread->mThread == pThread) {
            return thread;
        }
    }

    return 0;
}

#ifdef NON_MATCHING
JKRThread* JKRThreadSwitch::enter(JKRThread *pThread, int a2) {
    JKRThread* thread = pThread;

    if (!pThread) {
        return 0;
    }

    JKRThread* foundThread = JKRThread::searchThread(pThread->mThread);

    if (foundThread) {
        thread = foundThread;
    }

    JKRThread::TLoad* inf = &thread->_60;

    inf->clear();
    inf->_10 = a2;
    inf->_0 = 1;

    return thread;
}
#endif
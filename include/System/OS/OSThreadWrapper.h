#ifndef OSTHREADWRAPPER_H
#define OSTHREADWRAPPER_H

#include "types.h"
#include "JKernel/JKRHeap.h"
#include "revolution/os/OSMessage.h"
#include "revolution/os/OSThread.h"

class OSThreadWrapper
{
public:
    OSThreadWrapper(u32, s32, s32, JKRHeap *);

    virtual ~OSThreadWrapper();
    virtual s32 run();

    void initMessageQueue(JKRHeap *, s32);
    void initHeapSpecified(JKRHeap *, u32, s32 priority);
    s32 start(void *);

    JKRHeap* mHeap; // _4
    OSThread* mThreads; // _8
    OSMessageQueue mQueue; // _C
    OSMessage* mMessages; // _2C
    s32 mMessageCount; // _30
    void* mStack; // _34
    s32 stackSize; // _38
};

#endif // OSTHREADWRAPPER_H
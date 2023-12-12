#pragma once

#include <revolution.h>

class JKRHeap;

class OSThreadWrapper {
public:
    OSThreadWrapper(u32, int, int, JKRHeap *);

    virtual ~OSThreadWrapper();
    virtual s32 run();

    void initMessageQueue(JKRHeap *, int);
    void initHeapSpecified(JKRHeap *, u32, int);
    s32 start(void *);

    JKRHeap* _4;
    OSThread* mThread; // _8
    OSMessageQueue mQueue;  // _C
    OSMessage* mMessageArray;   // _2C
    u32 mMessageCount;      // _30
    u8* _34;
    u32 _38;
};
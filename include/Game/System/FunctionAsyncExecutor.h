#pragma once

#include "Game/System/OSThreadWrapper.h"
#include "Game/Util.h"

class FunctionAsyncExecInfo {
public:
    FunctionAsyncExecInfo(MR::FunctorBase *, int, const char *);
    ~FunctionAsyncExecInfo();

    void execute();

    MR::FunctorBase* mFunc;     // _0
    int mPriority;              // _4
    const char* _8;
    u8 _C;
    OSMessageQueue mQueue;      // _10
    OSMessage mMessage;         // _30
};

class FunctionAsyncExecutorThread : public OSThreadWrapper {
public:
    FunctionAsyncExecutorThread(JKRHeap *);

    virtual ~FunctionAsyncExecutorThread();
    virtual s32 run();

    u8 _3C;
    const char* _40;
};

class FunctionAsyncExecutorOnMainThread {
public:
    FunctionAsyncExecutorOnMainThread(OSThread *);

    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    OSMessageQueue mQueue;      // _4
    
};
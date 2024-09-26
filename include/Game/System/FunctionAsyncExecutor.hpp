#pragma once

#include "Game/System/OSThreadWrapper.hpp"
#include "Game/Util.hpp"

class JKRExpHeap;
class JKRUnitHeap;

class FunctionAsyncExecInfo {
public:
    FunctionAsyncExecInfo(MR::FunctorBase *, int, const char *);

    FunctionAsyncExecInfo() {

    }

    ~FunctionAsyncExecInfo();

    void execute();
    
    inline bool isSame(const char *pStr) {
        return MR::strcasecmp(_8, pStr) == 0;
    }

    MR::FunctorBase* mFunc;     // 0x0
    int mPriority;              // 0x4
    const char* _8;
    bool _C;
    OSMessageQueue mQueue;      // 0x10
    OSMessage mMessage;         // 0x30
};

class FunctionAsyncExecutorThread : public OSThreadWrapper {
public:
    FunctionAsyncExecutorThread(JKRHeap *);

    virtual ~FunctionAsyncExecutorThread();
    virtual s32 run();

    bool mIsSuspended;      // 0x3C
    const char* _40;
};

class FunctionAsyncExecutorOnMainThread {
public:
    FunctionAsyncExecutorOnMainThread(OSThread *);

    void update();

    u8 _0;
    u8 _1;
    u8 _2;
    u8 _3;
    OSMessageQueue mQueue;      // 0x4
    OSMessage mMsgArray[0x40];  // 0x24
    OSThread* mThread;          // 0x124
};

class FunctionAsyncExecutor {
public:
    FunctionAsyncExecutor();

    void update();
    void start(const MR::FunctorBase &, int, const char *);
    bool startOnMainThread(const MR::FunctorBase &, const char *);
    void waitForEnd(const char *);
    bool isEnd(const char *) const;
    OSThread* getOSThread(const char *);
    FunctionAsyncExecInfo* createAndAddExecInfo(const MR::FunctorBase &, int, const char *);
    FunctionAsyncExecutorThread* getSuspendThread();

    inline FunctionAsyncExecInfo* const* first() const {
        return &mHolders[0];
    }

    inline FunctionAsyncExecInfo* const* last() const {
        return &mHolders[_40C];
    }

    FunctionAsyncExecutorThread* mThreads[2];               // 0x0
    FunctionAsyncExecutorOnMainThread* mMainThreadExec;     // 0x8
    FunctionAsyncExecInfo* mHolders[0x100];
    u32 _40C;
    JKRUnitHeap* _410;
    JKRExpHeap* _414;
};
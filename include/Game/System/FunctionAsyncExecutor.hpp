#pragma once

#include "Game/System/OSThreadWrapper.hpp"
#include "Game/Util.hpp"

class JKRExpHeap;
class JKRUnitHeap;

class FunctionAsyncExecInfo {
public:
    FunctionAsyncExecInfo(MR::FunctorBase*, int, const char*);

    ~FunctionAsyncExecInfo();

    void execute();

    bool isSame(const char* pStr) { return MR::strcasecmp(mName, pStr) == 0; }

    /* 0x00 */ MR::FunctorBase* mFunc;
    /* 0x04 */ int mPriority;
    /* 0x08 */ const char* mName;
    /* 0x0C */ bool mIsEnd;
    /* 0x10 */ OSMessageQueue mQueue;
    /* 0x30 */ OSMessage mMessage;
};

class FunctionAsyncExecutorThread : public OSThreadWrapper {
public:
    FunctionAsyncExecutorThread(JKRHeap*);

    virtual void* run();

    /* 0x3C */ bool mIsSuspended;
    /* 0x40 */ const char* mName;
};

class FunctionAsyncExecutorOnMainThread {
public:
    FunctionAsyncExecutorOnMainThread(OSThread*);

    void update();

    /* 0x000 */ bool _0;
    /* 0x004 */ OSMessageQueue mQueue;
    /* 0x024 */ OSMessage mMsgArray[64];
    /* 0x124 */ OSThread* mThread;
};

class FunctionAsyncExecutor {
public:
    FunctionAsyncExecutor();

    void update();
    void start(const MR::FunctorBase&, int, const char*);
    bool startOnMainThread(const MR::FunctorBase&, const char*);
    void waitForEnd(const char*);
    bool isEnd(const char*) const;
    OSThread* getOSThread(const char*);
    FunctionAsyncExecInfo* createAndAddExecInfo(const MR::FunctorBase&, int, const char*);
    FunctionAsyncExecutorThread* getSuspendThread();

    /* 0x000 */ FunctionAsyncExecutorThread* mThreads[2];
    /* 0x008 */ FunctionAsyncExecutorOnMainThread* mMainThreadExec;
    /* 0x00C */ MR::Vector< MR::FixedArray< FunctionAsyncExecInfo*, 256 > > mHolders;
    /* 0x410 */ JKRUnitHeap* _410;
    /* 0x414 */ JKRExpHeap* _414;
};

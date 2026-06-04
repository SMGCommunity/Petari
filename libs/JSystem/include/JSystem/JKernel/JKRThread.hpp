#pragma once

#include "JSystem/JKernel/JKRDisposer.hpp"
#include "JSystem/JKernel/JKRHeap.hpp"

#include <revolution/os.h>

class JKRTask {
public:
};

class JKRThread : public JKRDisposer {
public:
    class TLoad {
    public:
        TLoad();

        void clear();

        u8 _0;
        u32 _4;
        u32 _8;
        u32 _C;
        u32 _10;
    };

    JKRThread(u32 stack_size, int message_count, int param_3);
    JKRThread(JKRHeap* heap, u32 stack_size, int message_count, int param_4);
    JKRThread(OSThread* thread, int message_count);

    virtual ~JKRThread();
    virtual void* run();

    void setCommon_mesgQueue(JKRHeap* heap, int message_count);
    void setCommon_heapSpecified(JKRHeap* heap, u32 stack_size, int param_3);

    OSThread* getThreadRecord() const {
        return mThread;
    }
    void* getStack() const {
        return mStackMemory;
    }
    TLoad* getLoadInfo() {
        return &mLoadInfo;
    }
    JKRHeap* getCurrentHeap() const {
        return mCurrentHeap;
    }
    s32 getCurrentHeapError() const {
        return mCurrentHeapError;
    }

    void setCurrentHeap(JKRHeap* heap) {
        mCurrentHeap = heap ? heap : JKRHeap::getCurrentHeap();
    }

    void resume() {
        OSResumeThread(mThread);
    }
    BOOL sendMessage(OSMessage message) {
        return OSSendMessage(&mMessageQueue, message, OS_MESSAGE_NOBLOCK);
    }
    void sendMessageBlock(OSMessage message) {
        OSSendMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }
    OSMessage waitMessage() {
        OSMessage message;
        OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_NOBLOCK);
        return message;
    }

    OSMessage waitMessage(BOOL* received) {
        OSMessage message;
        *received = OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_NOBLOCK);
        return message;
    }
    OSMessage waitMessageBlock() {
        OSMessage message;
        OSReceiveMessage(&mMessageQueue, &message, OS_MESSAGE_BLOCK);
        return message;
    }
    void jamMessageBlock(OSMessage message) {
        OSJamMessage(&mMessageQueue, message, OS_MESSAGE_BLOCK);
    }

    void* start(void* pData);

    static JKRThread* searchThread(OSThread*);

    /* 0x18 */ JSULink< JKRThread > mThreadListLink;
    /* 0x28 */ JKRHeap* mHeap;
    /* 0x2C */ OSThread* mThread;
    /* 0x30 */ OSMessageQueue mMessageQueue;
    /* 0x50 */ OSMessage* mMesgBuffer;
    /* 0x54 */ s32 mMessageCount;
    /* 0x58 */ void* mStackMemory;
    /* 0x5C */ u32 mStackSize;
    /* 0x60 */ TLoad mLoadInfo;
    /* 0x74 */ JKRHeap* mCurrentHeap;
    /* 0x78 */ s32 mCurrentHeapError;

    static JSUList< JKRThread > sThreadList;
    static JSUList< JKRTask > sTaskList;
};

class JKRThreadSwitch {
public:
    JKRThread* enter(JKRThread*, int);
};

#pragma once

#include "JSystem/JKernel/JKRDisposer.hpp"

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

    JKRThread(unsigned long, int, int);

    virtual ~JKRThread();
    virtual void* run();

    void setCommon_mesgQueue(JKRHeap*, int);
    void setCommon_heapSpecified(JKRHeap*, u32, int);
    void* start(void*);

    static JKRThread* searchThread(OSThread*);

    JSULink< JKRThread > mDisposerList;  // 0x18
    JKRHeap* _28;
    OSThread* mThread;      // 0x2C
    OSMessageQueue mQueue;  // 0x30
    OSMessage* mMessage;    // 0x50
    s32 mMsgCount;          // 0x54
    void* _58;
    void* _5C;
    JKRThread::TLoad _60;
    u32 _74;
    void* _78;

    static JSUList< JKRThread > sThreadList;
    static JSUList< JKRTask > sTaskList;
};

class JKRThreadSwitch {
public:
    JKRThread* enter(JKRThread*, int);
};

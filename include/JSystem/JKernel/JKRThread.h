#pragma once

#include "JSystem/JKernel/JKRDisposer.h"

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
    virtual s32 run();

    void setCommon_mesgQueue(JKRHeap *, int);
    void setCommon_heapSpecified(JKRHeap *, u32, int);
    s32 start(void *);

    static JKRThread* searchThread(OSThread *);

    JSULink<JKRThread> mDisposerList;     // _18
    JKRHeap* _28;
    OSThread* mThread;                      // _2C
    OSMessageQueue mQueue;                  // _30
    OSMessage* mMessage;                    // _50
    s32 mMsgCount;                          // _54
    void* _58;
    void* _5C;
    JKRThread::TLoad _60;
    u32 _74;
    u32 _78;

    static JSUList<JKRThread> sThreadList;
    static JSUList<JKRTask> sTaskList;
};

class JKRThreadSwitch {
public:
    JKRThread* enter(JKRThread *, int);
};
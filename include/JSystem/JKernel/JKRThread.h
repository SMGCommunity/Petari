#pragma once

#include "JSystem/JKernel/JKRDisposer.h"
#include <revolution/os.h>

class JKRThread : public JKRDisposer
{
public:
    class TLoad
    {
    public:
        TLoad();

        void clear();

        u8 _0;
        u32 _4;
        u32 _8;
        u32 _C;
        u32 _10;
    };

    JKRThread(u32, s32, s32);
    JKRThread(JKRHeap *, u32, s32, s32);

    virtual ~JKRThread();
    virtual s32 run()
    {
        return 0;
    }

    void setCommon_mesgQueue(JKRHeap *, s32);
    void setCommon_heapSpecified(JKRHeap *, u32, s32);
    void start(void *);
    OSThread* searchThread(OSThread *);
    OSThread* enter(JKRThread *, s32);

    JSUPtrLink _18;
    JKRHeap* _28;
    OSThread* mThread; // _2C
    OSMessageQueue mQueue; // _30
    OSMessage* mMessage; // _50
    s32 mMessageCount; // _54
    void* mStack; // _58
    u32 mStackSize; // _5C
    TLoad mLoad; // _60
    u32 _74;
    u32 _78;

    static JSUPtrList* sThreadList;
};
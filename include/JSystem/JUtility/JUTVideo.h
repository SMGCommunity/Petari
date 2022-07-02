#pragma once

#include <revolution.h>
#include <revolution/vi.h>

class JUTVideo {
public:
    JUTVideo(const _GXRenderModeObj *);

    virtual ~JUTVideo();

    _GXRenderModeObj* mRenderModeObj;   // _4
    u32 _8;
    u32 mRetraceCount;                  // _C
    u32 _10;
    u32 _14;
    u32 _18;
    VIRetraceCallback mPreRetractCb;    // _1C
    VIRetraceCallback mPostRetraceCb;   // _20
    u32 _24;
    u32 _28;
    u8 _2C;
    u8 _2D;
    u8 _2E;
    u8 _2F;
    u32 _30;
    OSMessage mMessage;                 // _34
    OSMessageQueue mMessageQueue;       // _38

    static JUTVideo* sManager;
};
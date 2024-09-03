#pragma once

#include <revolution.h>
#include <revolution/vi.h>

class JUTVideo {
public:
    JUTVideo(const _GXRenderModeObj *);

    virtual ~JUTVideo();

    static JUTVideo* createManager(const _GXRenderModeObj *);

    _GXRenderModeObj* mRenderModeObj;   // 0x4
    u32 _8;
    u32 mRetraceCount;                  // 0xC
    u32 _10;
    u32 _14;
    u32 _18;
    VIRetraceCallback mPreRetractCb;    // 0x1C
    VIRetraceCallback mPostRetraceCb;   // 0x20
    u32 _24;
    u32 _28;
    u8 _2C;
    u8 _2D;
    u8 _2E;
    u8 _2F;
    u32 _30;
    OSMessage mMessage;                 // 0x34
    OSMessageQueue mMessageQueue;       // 0x38

    static JUTVideo* sManager;
};
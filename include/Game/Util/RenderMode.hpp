#pragma once

#include <revolution.h>

class RenderModeObj {
public:
    static u32 getViWidthMax();

    VITVMode mTVMode;                    // 0x0
    u16 mFrameBufWidth;             // 0x4
    u16 mEFBHeight;                 // 0x6
    u16 mXFBHeight;                 // 0x8
    u16 mXOrigin;                   // 0xA
    u16 mYOrigin;                   // 0xC
    u16 mVIWidth;                   // 0xE
    u16 mVIHeight;                  // 0x10
    u16 _12;
    VIXFBMode mMode;                // 0x14
    u8 mFieldRendering;             // 0x18
    u8 mAA;                         // 0x19
    u8 mSamplePattern[12][2];       // 0x1A
    u8 mFilter[7];                  // 0x32
};

namespace MR {
    RenderModeObj* getSuitableRenderMode();
};
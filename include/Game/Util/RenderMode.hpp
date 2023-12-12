#pragma once

#include <revolution.h>

class RenderModeObj {
public:
    static u32 getViWidthMax();

    VITVMode mTVMode;                    // _0
    u16 mFrameBufWidth;             // _4
    u16 mEFBHeight;                 // _6
    u16 mXFBHeight;                 // _8
    u16 mXOrigin;                   // _A
    u16 mYOrigin;                   // _C
    u16 mVIWidth;                   // _E
    u16 mVIHeight;                  // _10
    u16 _12;
    VIXFBMode mMode;                // _14
    u8 mFieldRendering;             // _18
    u8 mAA;                         // _19
    u8 mSamplePattern[12][2];       // _1A
    u8 mFilter[7];                  // _32
};

namespace MR {
    RenderModeObj* getSuitableRenderMode();
};
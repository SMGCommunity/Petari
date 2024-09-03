#pragma once

#include "revolution.h"

class J3DAnmBase {
public:
    J3DAnmBase(s16);

    virtual ~J3DAnmBase();

    u8 mLoopMode;       // 0x4
    u8 _5;
    s16 mLoopDuration;  // 0x6
    f32 mCurrentFrame;  // 0x8
};

class J3DFrameCtrl {
public:

    inline J3DFrameCtrl(s16 endFrame) {
        init(endFrame);
    }

    virtual ~J3DFrameCtrl();

    void init(s16);
    bool checkPass(f32);
    void update();

    u8 mLoopMode;       // 0x4
    u8 mFlags;          // 0x5
    s16 mStartFrame;    // 0x6
    s16 mEndFrame;      // 0x8
    s16 mLoopFrame;     // 0xA
    f32 mSpeed;         // 0xC
    f32 mCurrentFrame;  // 0x10
};
#pragma once

#include "revolution.h"

class J3DAnmBase {
public:
    J3DAnmBase(s16);

    virtual ~J3DAnmBase();

    u8 mLoopMode;       // _4
    u8 _5;
    s16 mLoopDuration;  // _6
    f32 mCurrentFrame;  // _8
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

    u8 mLoopMode;       // _4
    u8 mFlags;          // _5
    s16 mStartFrame;    // _6
    s16 mEndFrame;      // _8
    s16 mLoopFrame;     // _A
    f32 mSpeed;         // _C
    f32 mCurrentFrame;  // _10
};
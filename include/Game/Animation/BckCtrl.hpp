#pragma once

#include <revolution.h>

class XanimePlayer;

class BckCtrlData {
public:
    // some callers inline this and some do not
    BckCtrlData() : _0(""), mPlayFrame(-1), mStartFrame(-1), mEndFrame(-1), mRepeatFrame(-1), mInterpole(-1), mLoopMode(0xff) {}

    void operator=(const BckCtrlData&);

    const char* _0;
    s16 mPlayFrame;    // 0x4
    s16 mStartFrame;   // 0x6
    s16 mEndFrame;     // 0x8
    s16 mRepeatFrame;  // 0xA
    s16 mInterpole;    // 0xC
    u8 mLoopMode;      // 0xE
    u8 _F;
    u8 _10;
    u8 _11;
};

class BckCtrlFunction {
public:
    static void reflectBckCtrlData(const BckCtrlData&, XanimePlayer*);
};

class BckCtrl {
public:
    BckCtrlData* find(const char*) const;
    void add(const BckCtrlData&);
    void overWrite(const BckCtrlData&);
    void changeBckSetting(const char*, XanimePlayer*) const;

    BckCtrlData mDefaultCtrlData;  // 0x0
    BckCtrlData* mControlData;     // 0x14
    u32 mControlDataCount;         // 0x18
    u32 _1C;
};

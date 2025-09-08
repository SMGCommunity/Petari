#pragma once
#include <revolution.h>

class XanimePlayer;

class BckCtrlData {
public:
    BckCtrlData()
        : _0(""),
          mPlayFrame(-1),
          mStartFrame(-1),
          mEndFrame(-1),
          mRepeatFrame(-1),
          mInterpole(-1),
          mLoopMode(0xFF),
          _F(0),
          _10(0),
          _11(0) {}

    void operator=(const BckCtrlData &other) {
        _0 = other._0;
        mPlayFrame = other.mPlayFrame;
        mStartFrame = other.mStartFrame;
        mEndFrame = other.mEndFrame;
        mRepeatFrame = other.mRepeatFrame;
        mInterpole = other.mInterpole;
        mLoopMode = other.mLoopMode;
        _F = other._F;
        _10 = other._10;
        _11 = other._11;
    }

    const char* _0;   // 0x00
    s16 mPlayFrame;   // 0x04
    s16 mStartFrame;  // 0x06
    s16 mEndFrame;    // 0x08
    s16 mRepeatFrame; // 0x0A
    s16 mInterpole;   // 0x0C
    u8  mLoopMode;    // 0x0E
    u8  _F;           // 0x0F
    u8  _10;          // 0x10
    u8  _11;          // 0x11
    // padding to 0x14 by compiler
};

class BckCtrlFunction {
public:
    static void reflectBckCtrlData(const BckCtrlData &, XanimePlayer *);
};

class BckCtrl {
public:
    BckCtrlData* find(const char* pName) const;
    void add(const BckCtrlData &);
    void overWrite(const BckCtrlData &);
    void changeBckSetting(const char*, XanimePlayer*) const;

    BckCtrlData  mDefaultCtrlData;   // 0x00..0x13
    BckCtrlData* mControlData;       // 0x14
    u32          _18;                // 0x18 (unused/pad)
    u32          mControlDataCount;  // 0x1C  ← must be here to match ASM
};

#pragma once

#include <revolution.h>

class XanimePlayer;

namespace {
    static const char* sDefaultPlayDataName = "_default";
};

class BckCtrlData { 
public:
    BckCtrlData();

    void operator=(const BckCtrlData &);

    const char* _0;
    s16 mPlayFrame;     // _4
    s16 mStartFrame;    // _6
    s16 mEndFrame;      // _8
    s16 mRepeatFrame;   // _A
    s16 mInterpole;     // _C
    u8 _E;
    u8 _F;
    u8 _10;
    u8 _11;
};

class BckCtrlFunction {
public:
    static void reflectBckCtrlData(const BckCtrlData &, XanimePlayer *);
};

class BckCtrl {
public:

    BckCtrlData* find(const char *) const;
    void add(const BckCtrlData &);
    void overWrite(const BckCtrlData &);
    void changeBckSetting(const char *, XanimePlayer *) const;

    BckCtrlData mDefaultCtrlData;   // _0
    BckCtrlData* mControlData;      // _14
    u32 mControlDataCount;          // _18
    u32 _1C;
};
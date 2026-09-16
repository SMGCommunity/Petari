#pragma once

#include "Game/Util/Array.hpp"
#include <revolution/types.h>

class ResourceHolder;
class XanimePlayer;

class BckCtrlData {
public:
    BckCtrlData() : mName(""), mPlayFrame(-1), mStartFrame(-1), mEndFrame(-1), mRepeatFrame(-1), mInterpole(-1), mLoopMode(-1) {
    }

    /* 0x00 */ const char* mName;
    /* 0x04 */ s16 mPlayFrame;
    /* 0x06 */ s16 mStartFrame;
    /* 0x08 */ s16 mEndFrame;
    /* 0x0A */ s16 mRepeatFrame;
    /* 0x0C */ s16 mInterpole;
    /* 0x0E */ u8 mLoopMode;
    /* 0x0F */ u8 _F;
    /* 0x10 */ u8 _10;
    /* 0x11 */ u8 _11;
};

class BckCtrlFunction {
public:
    static void reflectBckCtrlData(const BckCtrlData&, XanimePlayer*);
};

class BckCtrl {
public:
    BckCtrl(ResourceHolder*, const char*);

    BckCtrlData* find(const char*) const;
    void add(const BckCtrlData&);
    void overWrite(const BckCtrlData&);
    void changeBckSetting(const char*, XanimePlayer*) const;

    /* 0x00 */ BckCtrlData mDefaultCtrlData;
    /* 0x14 */ MR::Vector< MR::AssignableArray< BckCtrlData > > mControlData;
};

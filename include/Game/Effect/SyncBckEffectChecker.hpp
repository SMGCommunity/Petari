#pragma once

#include <revolution/types.h>

class SyncBckEffectInfo;
class XanimePlayer;

class SyncBckEffectChecker {
public:
    SyncBckEffectChecker(XanimePlayer* pPlayer);

    void updateBefore();
    void updateAfter();
    void reset();
    bool isCreate(const SyncBckEffectInfo* pInfo, bool checkFrame) const;
    bool isDelete(const SyncBckEffectInfo* pInfo) const;
    bool checkPass(f32 frame) const;
    bool checkPassIfRate0(f32 frame) const;

    /* 0x00 */ XanimePlayer* mPlayer;
    /* 0x04 */ f32 mPrevFrame;
    /* 0x08 */ bool mIsReset;
    /* 0x0C */ const char* mCurrentBckName;
    /* 0x10 */ const char* mPrevBckName;
};

#pragma once

#include <revolution/types.h>

class SyncBckEffectInfo;
class XanimePlayer;

class SyncBckEffectChecker {
public:
    SyncBckEffectChecker(XanimePlayer*);

    void updateBefore();
    void updateAfter();
    void reset();
    bool isCreate(const SyncBckEffectInfo*, bool) const;
    bool isDelete(const SyncBckEffectInfo*) const;
    bool checkPass(f32) const;
    bool checkPassIfRate0(f32) const;

    /* 0x00 */ XanimePlayer* _0;
    /* 0x04 */ f32 _4;
    /* 0x08 */ bool _8;
    /* 0x0C */ const char* _C;
    /* 0x10 */ const char* _10;
};

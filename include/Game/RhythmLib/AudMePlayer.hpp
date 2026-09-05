#pragma once

#include "Game/RhythmLib/AudMeTrack.hpp"
#include <JSystem/JAudio2/JASGlobal.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class AudMeHandle;

struct AudMePlayingParams {
    /* 0x0 */ u8 mPriority;
    /* 0x1 */ u8 mChordVolume;
    /* 0x2 */ u8 _2;
    /* 0x3 */ u8 _3;
    /* 0x4 */ u8 _4;
    /* 0x5 */ u8 _5;
    /* 0x6 */ u8 _6;
    /* 0x7 */ u8 _7;
};

class AudMePlayingParamsHolder {
public:
    void setResource(void*);

    /* 0x0 */ u32 mNumEntries;
    /* 0x4 */ AudMePlayingParams* mParams;
    /* 0x8 */ const char** mNames;
};

class AudMe : public JSULink< AudMe >, public JASPoolAllocObject< AudMe > {
public:
    AudMe();

    /* 0x10 */ u32 _10;
    /* 0x14 */ AudMeTrack* mTrack;
    /* 0x18 */ u32 _18;
    /* 0x1C */ u32 _1C;
    /* 0x20 */ s32 _20;
    /* 0x24 */ s32 _24;
    /* 0x28 */ u32 _28;
    /* 0x2C */ u8 pad[0x74 - 0x2C];
    /* 0x74 */ u32 _74;
    /* 0x78 */ u32 _78;
    /* 0x7C */ u32 _7C;
    /* 0x80 */ f32 _80;
};

class AudMeMgr : public JASGlobalInstance< AudMeMgr > {
public:
    AudMeMgr();

    void setMeSeq(void*);
    void setMeTable(void*);
    void startMe(u32, AudMeHandle*, const TVec3f*);
    void freeDeadMe();
    void updateEachMe();
    void stopAll(s32);
    void setPauseFlagAll(bool);
    void update();
    u32 getSeqStartPos(u32);
    bool isRequestedMe(u32);
    void setOuterPlayingParam(u32, AudMe*);

    u8 getPriority(int index) {
        return mParams.mParams[index].mPriority;
    }

    /* 0x00 */ u32 _0;
    /* 0x04 */ JSUList< AudMe > mMeList;
    /* 0x10 */ u32 _10[16];
    /* 0x50 */ f32 _50;
    /* 0x54 */ AudMePlayingParamsHolder mParams;
};

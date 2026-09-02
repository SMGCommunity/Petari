#pragma once

#include <JSystem/JAudio2/JASGlobal.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JSupport/JSUList.hpp>


class AudMeHandle;

class AudMePlayingParamsHolder {
public:
    void setResource(void*);

    /* 0x0 */ u32 mNumEntries;
    /* 0x4 */ u32 _4;
    /* 0x8 */ const char** mNames;
};

class AudMe : public JASPoolAllocObject< AudMe > {
public:
    AudMe();
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

    /* 0x00 */ u32 _0;
    /* 0x04 */ JSUList< AudMe > mMeList;
    /* 0x10 */ u32 _10[16];
    /* 0x50 */ f32 _50;
    /* 0x54 */ AudMePlayingParamsHolder mParams;
};

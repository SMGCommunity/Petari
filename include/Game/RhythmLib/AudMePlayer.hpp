#pragma once

#include <JSystem/JAudio2/JASGlobal.hpp>
#include <JSystem/JSupport/JSUList.hpp>

class AudMePlayingParamsHolder {
public:
    void setResource(void*);

    /* 0x0 */ u32 mNumEntries;
    /* 0x4 */ u32 _4;
    /* 0x8 */ const char** mNames;
};

class AudMe {
public:
    AudMe();
};

class AudMeMgr : public JASGlobalInstance< AudMeMgr > {
public:
    AudMeMgr();

    /* 0x00 */ u32 _0;
    /* 0x04 */ JSUList< AudMe > mMeList;
    /* 0x10 */ u32 _10[16];
    /* 0x50 */ f32 _50;
    /* 0x54 */ AudMePlayingParamsHolder mParams;
};

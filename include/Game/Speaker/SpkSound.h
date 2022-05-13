#pragma once

#include <revolution.h>
#include "JSystem/JSupport/JSUList.h"
#include "JSystem/JAudio2/JASGlobal.h"

class SpkSound;

class SpkSoundHandle {
public:
    void releaseSound();

    SpkSound* mSound;   // _0
};

class SpkSoundVolume {
public:
    SpkSoundVolume();

    void setRelease(s32);
    void setFadeOut(s32);
    f32 calc(bool &);

    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
};

class SpkSound {
public:
    SpkSound();
    ~SpkSound();

    void update(f32);
    void stop(s32);
    void attachHandle(SpkSoundHandle *);
    void releaseHandle();
    void start(s32, s32);
    void unlock();
    void setWaveData(s32);

    JSUPtrLink _0;
    SpkSoundHandle* mSoundHandle;   // _10
    s32 _14;
    u32 _18;
    u32 _1C;
    u32 _20;
    u32 _24;
    u32 _28;
    u32 _2C;
    s32 _30;
    s32 _34;
    u32 _38;
    s32 _3C;
    SpkSoundVolume mVolume;         // _40
};

class SpkSoundHolder : JASGlobalInstance<SpkSoundHolder> {
public:
    SpkSoundHolder();

    bool update(s32);
    bool updateEachSound(s32);

    void startSound(s32, s32, SpkSoundHandle *);

    void framework();

    JSUList<SpkSound> mSoundList[4];   // _0
    f32 _30[4];
    f32 _40;
    u32 _44;
};
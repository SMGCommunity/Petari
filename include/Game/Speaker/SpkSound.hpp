#pragma once

#include <JSystem/JAudio2/JASGlobal.hpp>
#include <JSystem/JAudio2/JASHeapCtrl.hpp>
#include <JSystem/JSupport/JSUList.hpp>
#include <revolution.h>

class SpkSound;

class SpkSoundHandle {
public:
    SpkSoundHandle() : mSound(nullptr) {
    }

    ~SpkSoundHandle() {
        releaseSound();
    };

    void releaseSound();

    inline SpkSound* getSound() {
        return mSound;
    }

    /* 0x00 */ SpkSound* mSound;
};

class SpkSoundVolume {
public:
    SpkSoundVolume();

    void setRelease(s32);
    void setFadeOut(s32);
    f32 calc(bool&);

    /* 0x00 */ f32 mReleaseVolume;
    /* 0x04 */ f32 mReleaseRate;
    /* 0x08 */ f32 mFadeOutVolume;
    /* 0x0C */ f32 mFadeOutRate;
    /* 0x10 */ f32 mFadeVolume;
    /* 0x14 */ f32 mFadeTarget;
    /* 0x18 */ f32 mFadeRate;
    /* 0x1C */ f32 mBaseVolume;
};

class SpkSound : public JSULink< SpkSound >, public JASPoolAllocObject< SpkSound > {
public:
    enum State {
        State_0 = 0,
        State_LOCK = 1,    // TODO: probably better name
        State_UNLOCK = 2,  // TODO: probably better name
        State_STOP = 3,
        State_DEAD = 4,
    };

    SpkSound();
    ~SpkSound();

    void update(f32);
    void stop(s32);
    void attachHandle(SpkSoundHandle*);
    void releaseHandle();
    bool start(s32, s32);
    void unlock();
    bool setWaveData(s32);

    bool isLoopValid() const {
        return mLoopStartPos >= 0 && mLoopEndPos > mLoopStartPos;
    }

    /* 0x10 */ SpkSoundHandle* mSoundHandle;
    /* 0x14 */ s32 mWaveID;
    /* 0x18 */ s16* mWave;
    /* 0x1C */ s32 mWavePtr;
    /* 0x20 */ s32 mPadChannel;
    /* 0x24 */ u32 _24;
    /* 0x28 */ u32 mReleaseTime;
    /* 0x2C */ s32 mWaveSize;
    /* 0x30 */ s32 mLoopStartPos;
    /* 0x34 */ s32 mLoopEndPos;
    /* 0x38 */ s32 mState;
    /* 0x3C */ s32 _3C;
    /* 0x40 */ SpkSoundVolume mVolume;
};

class SpkSoundHolder : JASGlobalInstance< SpkSoundHolder > {
public:
    SpkSoundHolder();

    bool startSound(s32, s32, SpkSoundHandle*);
    bool update(s32);
    bool updateEachSound(s32);
    void freeDeadSound(s32);
    void framework();

    f32 getBaseVolume() const {
        return mBaseVolume;
    }

    f32 get_44() const {
        return _44 / 15.0f;
    }

    /* 0x00 */ JSUList< SpkSound > mSoundList[4];
    /* 0x30 */ f32 mVolume[4];
    /* 0x40 */ f32 mBaseVolume;
    /* 0x44 */ s32 _44;
};

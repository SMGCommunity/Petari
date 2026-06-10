#pragma once

#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class JASSoundParams;
class JAISoundHandles;

class JAUSoundAnimationSound {
public:
    // TODO: why are some normal rlwinm and other extri and some dont matter?
    bool playsOnlyForward() const {
        return (mFlags & 1) != 0;  // 0x01
    }
    bool playsOnlyReverse() const {
        return (mFlags & 2) != 0;  // 0x02
    }
    bool stopsWhenAnimationChanges() const {
        return (mFlags & 4) != 0;  // 0x04
    }
    bool playsOnlyOnce() const {
        return (mFlags & 8) != 0;  // 0x08
    }
    bool stopsWhenNoteOff() const {
        return (mFlags & 0x10) != 0;  // 0x10
    }
    bool stopsWhenSpeedIsZero() const {
        return (mFlags & 0x20) != 0;  // 0x20
    }
    bool setsLifeTime() const {
        return (mFlags >> 6) & 1;  // 0x40
    }
    bool playsAtIntervals() const {
        return (mFlags & 0x80) != 0;  // 0x80
    }

    bool isNoting(f32 time) const {
        if (mNoteOnTime == mNoteOffTime) {
            return true;
        }
        if (mNoteOnTime > mNoteOffTime) {
            if (mNoteOffTime <= time && time < mNoteOnTime) {
                return true;
            }
        } else {
            if (mNoteOnTime <= time && time < mNoteOffTime) {
                return true;
            }
        }
        return false;
    }

    bool isNotingOn(f32 time, bool reversed) const {
        if (setsLifeTime()) {
            return isNoting(time);
        }
        if (reversed) {
            return time <= mNoteOnTime;
        } else {
            return time >= mNoteOnTime;
        }
    }

    bool isNotingOff(f32 time, bool reversed) const {
        if (setsLifeTime()) {
            return !isNoting(time);
        }
        if (mNoteOnTime == mNoteOffTime) {
            return false;
        }
        if (reversed) {
            return time <= mNoteOffTime;
        } else {
            return time >= mNoteOffTime;
        }
    }

    JAISoundID getSoundID() const {
        return mSoundID;
    }

    /* 0x00 */ JAISoundID mSoundID;
    /* 0x04 */ f32 mNoteOnTime;
    /* 0x08 */ f32 mNoteOffTime;
    /* 0x0C */ f32 mBasePitch;
    /* 0x10 */ u32 mFlags;
    /* 0x14 */ u8 mBaseVolume;
    /* 0x15 */ s8 mPitchDelta;
    /* 0x16 */ u8 mPlayTime;
    /* 0x17 */ u8 mBasePan;
    /* 0x18 */ s8 mVolumeDelta;
    /* 0x19 */ u8 mRepeatInterval;
    /* 0x1A */ s8 _1A;
    /* 0x1B */ u8 _1B[5];
};

class JAUSoundAnimation;

class JAUSoundAnimationControl {
public:
    virtual ~JAUSoundAnimationControl() = 0;
    virtual JAUSoundAnimationSound* getSound(const JAUSoundAnimation*, int) = 0;
    virtual u16 getNumSounds(const JAUSoundAnimation*) = 0;
};

class JAUSoundAnimation {
public:
    u32 getStartSoundIndex(f32) const;
    u32 getEndSoundIndex(f32) const;
    u16 getNumSounds() const {
        if (mControl != nullptr) {
            return mControl->getNumSounds(this);
        } else {
            return mNumSounds;
        }
    }

    const JAUSoundAnimationSound* getSound(int i_index) const {
        if (mControl != nullptr) {
            return mControl->getSound(this, i_index);
        } else {
            return &mSounds + i_index;
        }
    }

    /* 0x0 */ u16 mNumSounds;
    /* 0x4 */ JAUSoundAnimationControl* mControl;
    /* 0x8 */ JAUSoundAnimationSound mSounds;
};

class JAUSoundAnimator {
public:
    JAUSoundAnimator(JAISoundHandles*);

    virtual JAISoundHandle* getFreeHandle(const JAUSoundAnimationSound* pAnimation);
    virtual u32 getSoundID(const JAUSoundAnimationSound* pAnimation, const TVec3f& rPos, f32);
    virtual bool playsSound(const JAUSoundAnimationSound* pAnimation, const TVec3f& rPos, f32);
    virtual void modifySoundParams(JASSoundParams* pParams, const JAUSoundAnimationSound* pAnimation, f32 time);

    void ageSounds_();
    void setLoopFrame(f32 loopStartFrame, f32 loopEndFrame) {
        mLoopStartFrame = loopStartFrame;
        mLoopStartSoundIndex = mSoundAnimation->getStartSoundIndex(loopStartFrame);
        mLoopEndFrame = loopEndFrame;
        mLoopEndSoundIndex = mSoundAnimation->getEndSoundIndex(loopEndFrame);
    }
    void startAnimation(const JAUSoundAnimation* pAnimation, bool reversed, f32 loopStartFrame, f32 loopEndFrame);
    void removeAnimation();
    void updateSoundLifeTime_(f32 loopStartFrame, f32 loopEndFrame);

    JAISound* getSound(int index) {
        return mHandles->getHandle(index)->getSound();
    }

    /* 0x04 */ JAISoundHandles* mHandles;
    /* 0x08 */ const JAUSoundAnimation* mSoundAnimation;
    /* 0x0C */ bool mReversed;
    /* 0x10 */ u32 mMaxSoundNo;
    /* 0x14 */ f32 _14;  // something "max"
    /* 0x18 */ u32 mLoopStartSoundIndex;
    /* 0x1C */ u32 mLoopEndSoundIndex;
    /* 0x20 */ f32 mLoopStartFrame;
    /* 0x24 */ f32 mLoopEndFrame;
    /* 0x28 */ s32 mTime;
};

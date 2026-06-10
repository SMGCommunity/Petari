#include "JSystem/JAudio2/JAUSoundAnimator.hpp"
#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"

u32 JAUSoundAnimation::getStartSoundIndex(f32 time) const {
    int idx;
    for (idx = 0; idx < getNumSounds(); idx++) {
        if (getSound(idx)->mNoteOnTime >= time) {
            break;
        }
    }
    return idx;
}

u32 JAUSoundAnimation::getEndSoundIndex(f32 time) const {
    int idx;
    for (idx = 0; idx < getNumSounds(); idx++) {
        if (getSound(idx)->mNoteOnTime > time) {
            break;
        }
    }
    return idx;
}

JAUSoundAnimator::JAUSoundAnimator(JAISoundHandles* pHandles) : mSoundAnimation(nullptr) {
    mHandles = pHandles;
    mReversed = false;
}

void JAUSoundAnimator::removeAnimation() {
    mSoundAnimation = nullptr;
}

void JAUSoundAnimator::ageSounds_() {
    JAISoundHandles* handles = mHandles;
    for (int idx = 0; idx < handles->mNumHandles; idx++) {
        JAISound* sound = handles->getHandle(idx)->getSound();
        if (sound == nullptr) {
            continue;
        }

        switch (sound->getAnimationState()) {
        case 0:
            break;
        case 1:
            if (sound->hasLifeTime()) {
                sound->stop();
                continue;
            }
            if (((JAUSoundAnimationSound*)sound->getUserData())->stopsWhenAnimationChanges()) {
                sound->stop();
                continue;
            }
            sound->setAnimationState(2);
            break;
        case 2:
        case 3:
            break;
        }
    }
}

void JAUSoundAnimator::startAnimation(const JAUSoundAnimation* pAnimation, bool reversed, f32 loopStartFrame, f32 loopEndFrame) {
    ageSounds_();
    mSoundAnimation = pAnimation;
    if (mSoundAnimation == nullptr) {
        return;
    }

    mReversed = reversed;
    mTime = 0;

    if (mReversed) {
        mMaxSoundNo = mSoundAnimation->getNumSounds() - 1;
        _14 = FLOAT_MAX;
    } else {
        mMaxSoundNo = 0;
        _14 = 0.0f;
    }

    setLoopFrame(loopStartFrame, loopEndFrame);
}

void JAUSoundAnimator::updateSoundLifeTime_(f32 time, f32 speed) {
    if (mSoundAnimation == nullptr) {
        return;
    }

    JAISoundHandles* handles = mHandles;

    for (int idx = 0; idx < handles->mNumHandles; idx++) {
        if (handles->getSound(idx) == nullptr) {
            continue;
        }

        switch (handles->getSound(idx)->getAnimationState()) {
        case 0:
            continue;
        case 1:
            break;
        case 2:
        case 3:
            continue;
        default:
            continue;
        }

        JAUSoundAnimationSound* anim = (JAUSoundAnimationSound*)handles->getSound(idx)->getUserData();

        bool speedZeroStop = speed == 0.0f && anim->stopsWhenSpeedIsZero();
        modifySoundParams(&handles->getSound(idx)->mParams.getMoveParams(), anim, speed);

        if (anim->setsLifeTime()) {
            if (speedZeroStop) {
                handles->getSound(idx)->pause(true);
                continue;
            }

            if (anim->isNoting(time)) {
                if (handles->getSound(idx)->mLifeTime < 1) {
                    handles->getSound(idx)->mLifeTime = 1;
                }

                handles->getSound(idx)->pause(false);
            } else {
                handles->getSound(idx)->stop();
            }
        } else {
            if (speedZeroStop) {
                handles->getSound(idx)->stop();
                continue;
            }

            if (anim->stopsWhenNoteOff() && anim->isNotingOff(time, mReversed)) {
                handles->getSound(idx)->stop();
            }
        }
    }
}

JAISoundHandle* JAUSoundAnimator::getFreeHandle(const JAUSoundAnimationSound* pAnimation) {
    for (int idx = 0; idx < mHandles->mNumHandles; idx++) {
        if (mHandles->getSound(idx) == nullptr) {
            return mHandles->getHandle(idx);
        }
    }
    return nullptr;
}

u32 JAUSoundAnimator::getSoundID(const JAUSoundAnimationSound* pAnimation, const TVec3f& rPos, f32 f1) {
    return pAnimation->getSoundID();
}

bool JAUSoundAnimator::playsSound(const JAUSoundAnimationSound* pAnimation, const TVec3f& rPos, f32 f1) {
    if (pAnimation->playsOnlyOnce()) {
        if (mTime != pAnimation->mPlayTime) {
            return false;
        }
    } else if (pAnimation->playsAtIntervals() && pAnimation->mPlayTime != mTime % pAnimation->mRepeatInterval) {
        return false;
    }

    if (mReversed) {
        if (pAnimation->playsOnlyForward()) {
            return false;
        }
    } else {
        if (pAnimation->playsOnlyReverse()) {
            return false;
        }
    }

    return true;
}

void JAUSoundAnimator::modifySoundParams(JASSoundParams* pParams, const JAUSoundAnimationSound* pAnimation, f32 time) {
    time = time < 0.0f ? -time - 1.0f : time - 1.0f;

    f32 volume = pAnimation->mBaseVolume;
    if (pAnimation->mVolumeDelta != 0) {
        volume += pAnimation->mVolumeDelta * 2.0f * time;
    }

    pParams->mVolume = volume * (1.0f / 127.0f);

    if (pAnimation->mPitchDelta != 0) {
        pParams->mPitch = pAnimation->mBasePitch + pAnimation->mPitchDelta * time * (1.0f / 32.0f);
    } else {
        pParams->mPitch = pAnimation->mBasePitch;
    }

    pParams->mPan = pAnimation->mBasePan * (1.0f / 127.0f);
}

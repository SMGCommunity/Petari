#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/Boss/BossStinkBug.hpp"
#include "Game/Enemy/Kanina.hpp"
#include "Game/NPC/Rabbit.hpp"
#include "Game/Player/Mario.hpp"
#include <JSystem/JAudio2/JAISound.hpp>
#include <JSystem/JAudio2/JAUSoundAnimator.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>

void AudAnmSoundObject_FORCE_MATCH_SDATA2() {
    (void)0.0f;
}

AudAnmSoundObject::AudAnmSoundObject(TVec3f* pPos, u8 numHandles, JKRHeap* pHeap) : AudSoundObject(pPos, numHandles, pHeap), JAUSoundAnimator(this) {
}

void AudAnmSoundObject::update(f32 time) {
    if (mPos == nullptr) {
        return;
    }

    updateAnimSound(time, *mPos, nullptr);
}

void AudAnmSoundObject::modifySoundParams(JASSoundParams* pParams, const JAUSoundAnimationSound* pSound, f32 speed) {
    if (speed > 3.0f) {
        speed = 3.0f;
    }
    JAUSoundAnimator::modifySoundParams(pParams, pSound, speed);
}

bool AudAnmSoundObject::playsSound(const JAUSoundAnimationSound* pSound, const TVec3f& rPos, f32 speed) {
    if (!JAUSoundAnimator::playsSound(pSound, rPos, speed)) {
        return false;
    }

    return !isLimitedSound(getSoundID(pSound, rPos, speed));
}

u32 AudAnmSoundObject::getSoundID(const JAUSoundAnimationSound* pSound, const TVec3f& rPos, f32 speed) {
    switch (pSound->getSoundID()) {
    case SE_EM_KANINA_STEP: {
        if (mSeVersion != Kanina::SeVersion_NORMAL) {
            return SE_EM_KANINA_STEP_W;
        }
        break;
    }
    case SE_SM_RABBIT_HOP: {
        if (mSeVersion != Rabbit::SeVersion_NORMAL) {
            return SE_SM_RABBIT_HOP_WATER;
        }
        break;
    }
    case SE_SM_RABBIT_JUMP: {
        if (mSeVersion != Rabbit::SeVersion_NORMAL) {
            return SE_SM_RABBIT_JUMP_WATER;
        }
        break;
    }
    case SE_BM_BOSS_BUG_FOOT_NOTE1: {
        if (mSeVersion == BossStinkBug::SeVersion_WATER || mSeVersion == BossStinkBug::SeVersion_ANGRY_WATER) {
            return SE_BM_BOSS_BUG_FOOT_NOTE1W;
        }
        break;
    }
    case SE_BM_BOSS_BUG_FOOT_NOTE_RUN: {
        if (mSeVersion == BossStinkBug::SeVersion_ANGRY || mSeVersion == BossStinkBug::SeVersion_ANGRY_WATER) {
            return SE_BM_BOSS_BUG_FOOT_NOTE_ANGRY;
        }
        break;
    }
    case SE_PM_STAR_POWER_GET_1: {
        return -1;
        break;
    }
    case SE_SV_KPVS3_PEACH_CALL_MARIO: {
        if (AudWrap::getSceneMgr()->isPlayerModeLuigi()) {
            return SE_SV_KPVS3_PEACH_CALL_LUIGI;
        }
        break;
    }
    case SE_PM_FALLDOWN_S:
    case SE_PM_FALLDOWN_M:
    case SE_PM_FOOTNOTE_L:
    case SE_PM_FOOTNOTE_R: {
        if (mSeVersion == Mario::SeVersion_DARK) {
            return -1;
        }
        break;
    }
    }

    return convertSoundIdFromSeVersion(pSound->getSoundID());
}

JAISoundHandle* AudAnmSoundObject::getFreeHandle(const JAUSoundAnimationSound* pSound) {
    return JAUSoundAnimator::getFreeHandle(pSound);
}

void AudAnmSoundObject::skip(f32 time) {
    if (mSoundAnimation == nullptr || mSoundAnimation->getNumSounds() == 0) {
        return;
    }

    f32 speed = time - mLifeTime;
    if (!mIsReversed) {
        if (speed < 0.0f) {
            speed += mLoopEndFrame - mLoopStartFrame;
            while (mLoopSoundIndex < mLoopEndSoundIndex) {
                if (mIsReversed) {
                    mLoopSoundIndex--;
                } else {
                    mLoopSoundIndex++;
                }
            }
            mLifeTime = time;
            mLoopSoundIndex = mLoopStartSoundIndex;
            if (mTime < 0xFFFF) {
                mTime++;
            }
        }
        updateSoundLifeTime_(time, speed);
        while (mLoopSoundIndex < mSoundAnimation->getNumSounds() && mSoundAnimation->getSound(mLoopSoundIndex)->isNotingOn(time, false)) {
            if (mIsReversed) {
                mLoopSoundIndex--;
            } else {
                mLoopSoundIndex++;
            }
        }
    } else {
        if (speed > 0.0f) {
            speed -= mLoopEndFrame - mLoopStartFrame;
            while (mLoopSoundIndex >= mLoopStartSoundIndex) {
                if (mIsReversed) {
                    mLoopSoundIndex--;
                } else {
                    mLoopSoundIndex++;
                }
            }
            mLifeTime = time;
            mLoopSoundIndex = mLoopEndSoundIndex - 1;
            if (mTime < 0xFFFF) {
                mTime++;
            }
        }
        updateSoundLifeTime_(time, speed);
        while (mLoopSoundIndex >= 0 && mSoundAnimation->getSound(mLoopSoundIndex)->isNotingOn(time, true)) {
            if (mIsReversed) {
                mLoopSoundIndex--;
            } else {
                mLoopSoundIndex++;
            }
        }
    }

    mLifeTime = time;
}

void AudAnmSoundObject::setStartPos(f32 time) {
    if (mSoundAnimation == nullptr || mSoundAnimation->getNumSounds() == 0) {
        return;
    }

    mLifeTime = time;
    mLoopSoundIndex = mSoundAnimation->getStartSoundIndex(time);
    if (mIsReversed && mLoopSoundIndex > 0) {
        mLoopSoundIndex--;
    }
}

void AudAnmSoundObject::updateAnimSound(f32 time, const TVec3f& rPos, JAISoundStarter* pStarter) {
    JAISoundHandles* handles = getAnimatorHandles();
    for (s32 index = 0; index < handles->getNumHandles(); index++) {
        JAISound* sound = handles->getSound(index);
        if (sound != nullptr && sound->getAnimationState() != 0) {
            sound->setPos(rPos);
        }
    }

    if (mSoundAnimation == nullptr || mSoundAnimation->getNumSounds() == 0) {
        return;
    }

    if (pStarter == nullptr) {
        pStarter = JAISoundStarter::getInstance();
    }

    f32 speed = time - mLifeTime;
    if (!mIsReversed) {
        if (speed < 0.0f) {
            speed += mLoopEndFrame - mLoopStartFrame;
            while (mLoopSoundIndex < mLoopEndSoundIndex) {
                startAnimSound(rPos, speed, pStarter);
            }
            mLifeTime = time;
            mLoopSoundIndex = mLoopStartSoundIndex;
            if (mTime < 0xFFFF) {
                mTime++;
            }
        }
        updateSoundLifeTime_(time, speed);
        while (mLoopSoundIndex < mSoundAnimation->getNumSounds() && mSoundAnimation->getSound(mLoopSoundIndex)->isNotingOn(time, false)) {
            startAnimSound(rPos, speed, pStarter);
        }
    } else {
        if (speed > 0.0f) {
            speed -= mLoopEndFrame - mLoopStartFrame;
            while (mLoopSoundIndex >= mLoopStartSoundIndex) {
                startAnimSound(rPos, speed, pStarter);
            }
            mLifeTime = time;
            mLoopSoundIndex = mLoopEndSoundIndex - 1;
            if (mTime < 0xFFFF) {
                mTime++;
            }
        }
        updateSoundLifeTime_(time, speed);
        while (mLoopSoundIndex >= 0 && mSoundAnimation->getSound(mLoopSoundIndex)->isNotingOn(time, true)) {
            startAnimSound(rPos, speed, pStarter);
        }
    }

    mLifeTime = time;
}

void AudAnmSoundObject::startAnimSound(const TVec3f& rPos, f32 speed, JAISoundStarter* pStarter) {
    const JAUSoundAnimationSound* sound = mSoundAnimation->getSound(mLoopSoundIndex);
    if (mIsReversed) {
        mLoopSoundIndex--;
    } else {
        mLoopSoundIndex++;
    }

    JAISoundHandle* handle = getAnimatorHandles()->getHandleUserData((u32)sound);
    if (handle == nullptr) {
        handle = getFreeHandle(sound);
    }

    if (handle == nullptr) {
        return;
    }

    u32 soundID = getSoundID(sound, rPos, speed);
    if (soundID == -1) {
        return;
    }

    if (!playsSound(sound, rPos, speed)) {
        return;
    }

    if (!pStarter->startSound(soundID, handle, &rPos)) {
        return;
    }

    if (!handle->isSoundAttached()) {
        return;
    }

    handle->getSound()->setAnimationState(1);
    handle->getSound()->setUserData((u32)sound);

    setMapCodeToPort(handle, soundID);
    setCutoffToPort(handle, soundID);

    if (sound->setsLifeTime()) {
        handle->getSound()->setLifeTime(1, false);
    }

    modifySoundParams(&handle->getSound()->getAuxiliary().mParams, sound, speed);
    releaseHandleIfNecessary(handle, soundID);
}

bool AudAnmSoundObject::releaseHandleIfNecessary(JAISoundHandle* pHandle, u32 soundID) {
    if (pHandle == nullptr) {
        return false;
    }

    if (!pHandle->isSoundAttached()) {
        return false;
    }

    switch (soundID) {
    case SE_PV_LIBRARY_SURPRISE:
        pHandle->getSound()->releaseHandle();
        return true;
    }
    return false;
}

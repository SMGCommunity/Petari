#include "JSystem/JAudio2/JAISound.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAISoundHandles.hpp"
#include "JSystem/JAudio2/JASReport.hpp"
#include "JSystem/JAudio2/JASTrack.hpp"

void JAISoundHandle::releaseSound() {
    if (mSound != nullptr) {
        mSound->mHandle = nullptr;
        mSound = nullptr;
    }
}

void JAISound::releaseHandle() {
    if (mHandle != nullptr) {
        mHandle->mSound = nullptr;
        mHandle = nullptr;
    }
}

void JAISound::attachHandle(JAISoundHandle* handle) {
    if (handle->isSoundAttached()) {
        handle->getSound()->stop();
    }
    if (isHandleAttached()) {
        releaseHandle();
    }
    mHandle = handle;
    mHandle->mSound = this;
}

s32 JAISoundStatus_::lockWhenPrepared() {
    if (getState() == State_PREPARE) {
        setState(State_LOCK_PREPARE);
        return 1;
    }
    return 0;
}

s32 JAISoundStatus_::unlockIfLocked() {
    if (getState() == State_LOCK_READY) {
        setState(State_READY);
        return 1;
    }
    if (getState() == State_LOCK_PREPARE) {
        setState(State_PREPARE);
        return 1;
    }
    return 0;
}

void JAISoundParams::mixOutAll(const JASSoundParams& inParams, JASSoundParams* outParams, f32 param_2) {
    outParams->mVolume = mMove.mParams.mVolume * (inParams.mVolume * mProperty._0) * param_2;
    outParams->mFxMix = mMove.mParams.mFxMix + (inParams.mFxMix + mProperty._4);
    outParams->mPitch = mMove.mParams.mPitch * (inParams.mPitch * mProperty._8);
    outParams->mPan = (inParams.mPan + mMove.mParams.mPan) - 0.5f;
    outParams->mDolby = inParams.mDolby + mMove.mParams.mDolby;
}

JAISound::JAISound() : mParams() {
}

void JAISound::start_JAISound_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience) {
    mHandle = nullptr;
    mSoundID = id;
    mStatus.init();
    mParams.init();
    mFader.forceIn();
    mAudience = audience;
    mPrepareCount = 0;
    mCount = 0;

    if (posPtr != nullptr && mAudience != nullptr) {
        mAudible = mAudience->newAudible(*posPtr, mSoundID, nullptr, 0);
    } else {
        mAudible = nullptr;
    }

    mPriority = 0;
}

bool JAISound::acceptsNewAudible() const {
    return mAudible == nullptr && !isCalcedOnce();
}

void JAISound::newAudible(const TVec3f& pos, const TVec3f* param_1, u32 param_2, JAIAudience* audience) {
    if (audience != nullptr) {
        mAudience = audience;
    }

    mAudible = mAudience->newAudible(pos, mSoundID, param_1, param_2);
}

void JAISound::stop(u32 fadeTime) {
    if (fadeTime == 0) {
        stop();
    } else {
        mFader.fadeOut(fadeTime);
        removeLifeTime_();
        mStatus.mState.flags.flag5 = 1;
        mStatus.mState.flags.flag1 = 1;
    }
}

void JAISound::stop() {
    stop_JAISound_();
    releaseHandle();
}

JAISe* JAISound::asSe() {
    return nullptr;
}

JAISeq* JAISound::asSeq() {
    return nullptr;
}

JAIStream* JAISound::asStream() {
    return nullptr;
}

void JAISound::die_JAISound_() {
    if (mAudible != nullptr) {
        mAudience->deleteAudible(mAudible);
        mAudible = nullptr;
        mAudience = nullptr;
    }
    mFader.forceOut();
    releaseHandle();
    die();
}

void JAISound::increasePrepareCount_JAISound_() {
    if ((++mPrepareCount & 0xFF) == 0) {
        JASReport("It cost %d steps to prepare Sound(ID:%08x, Address%08x).\n", mPrepareCount, *(u32*)&mSoundID, this);
    }
}

bool JAISound::calc_JAISound_() {
    setCalcedOnce(true);
    if (isStopping() && JAISound_tryDie_()) {
        return false;
    }

    bool playing = isPlaying();
    if (playing) {
        mCount++;
    }

    bool paused = mStatus.isPaused();
    if (!paused) {
        mFader.calc();
    }

    bool currentlyPlaying = playing && !paused;
    if (currentlyPlaying) {
        mParams.mMove.calc();
        if (mAudible != nullptr) {
            mAudible->calc();
        }
        if (hasLifeTime()) {
            if (mLifeTime == 0) {
                stop_JAISound_();
            } else {
                mLifeTime--;
            }
        }
    }

    if (mAudience != nullptr && mAudible != nullptr) {
        mPriority = mAudience->calcPriority(mAudible);
        if (mPriority == -1 && !comesBack()) {
            stop_JAISound_();
        }
    } else {
        mPriority = 0;
    }

    return currentlyPlaying;
}

void JAISound::initTrack_JAISound_(JASTrack* track) {
    JASSoundParams* soundParams[JASTrack::TChannelMgr::CHANNEL_MAX];

    int numChannels = 0;

    for (int i = 0; i < mAudience->getMaxChannels(); i++) {
        JASSoundParams* currentParams = mAudible->getOuterParams(i);
        if (currentParams != nullptr) {
            soundParams[numChannels++] = currentParams;
        }
    }

    track->setChannelMgrCount(numChannels);

    for (u32 i = 0; i < track->getChannelMgrCount(); i++) {
        track->assignExtBuffer(i, soundParams[i]);
    }
}

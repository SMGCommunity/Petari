#include "JSystem/JAudio2/JAIStream.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAISoundChild.hpp"
#include "JSystem/JAudio2/JAIStreamDataMgr.hpp"
#include "JSystem/JAudio2/JAIStreamMgr.hpp"
#include "JSystem/JAudio2/JASAramStream.hpp"

static void JAIStream_JASAramStreamCallback_(u32 type, JASAramStream* aramStream, void* userData) {
    JAIStream* stream = (JAIStream*)userData;

    switch (type) {
    case JASAramStream::CB_STOP:
        stream->mIsStreamStopped = true;
        break;
    case JASAramStream::CB_START:
        stream->mIsStreamStarted = true;
        break;
    }
}

JAIStream::JAIStream(JAIStreamMgr* streamMgr_, JAISoundStrategyMgr< JAIStream >* soundStrategyMgr_) : JSULink< JAIStream >(this) {
    // FIXME: out-of-order reg load
    mIsStreamStopped = false;
    mPrepareState = 0;
    soundStrategyMgr = soundStrategyMgr_;
    streamMgr = streamMgr_;
    streamAramAddr = nullptr;
    mIsStreamStarted = false;

    for (int i = 0; i < NUM_CHILDREN; i++) {
        children[i] = nullptr;
    }
}

void JAIStream::JAIStreamMgr_startID_(JAISoundID id, s32 streamFileEntry, const TVec3f* posPtr, JAIAudience* audience, int category) {
    mCategory = category;
    mStreamFileEntry = streamFileEntry;
    start_JAISound_(id, posPtr, audience);
    mPrepareState = 0;

    if (soundStrategyMgr != nullptr) {
        soundStrategy = soundStrategyMgr->calc(id);
    } else {
        soundStrategy = nullptr;
    }
}

bool JAIStream::prepare_prepareStream_() {
    u32 local_28;
    JAIStreamAramMgr* streamAramMgr;

    switch (mPrepareState) {
    case 0:
        streamAramMgr = streamMgr->getStreamAramMgr();

        streamAramAddr = streamAramMgr->newStreamAram(&local_28);
        if (streamAramAddr != nullptr) {
            inner_.aramStream.init((u32)streamAramAddr, local_28, &JAIStream_JASAramStreamCallback_, this);
            mPrepareState = 1;
            mPrepareCount = 0;
        } else {
            increasePrepareCount_JAISound_();
        }
        break;
    case 1:
        if (mAudible != nullptr) {
            JASSoundParams* soundParams = mAudible->getOuterParams(0);
            inner_.aramStream.setPitch(soundParams->mPitch);
            inner_.aramStream.setVolume(soundParams->mVolume);
            inner_.aramStream.setPan(soundParams->mPan);
            inner_.aramStream.setFxmix(soundParams->mFxMix);
            inner_.aramStream.setDolby(soundParams->mDolby);
        }

        mIsStreamStopped = false;

        if (inner_.aramStream.prepare(mStreamFileEntry, -1)) {
            mPrepareState = 2;
        }
        break;
    case 2:
        if (mIsStreamStopped) {
            mIsStreamStopped = false;
            mPrepareState = 3;
        } else {
            increasePrepareCount_JAISound_();
        }
        break;
    case 3:
        return true;
    case 4:
        break;
    }

    return false;
}

void JAIStream::prepare_() {
    if (mIsStreamStarted) {
        return;
    }

    switch (mStatus.getState()) {
    case JAISoundStatus_::State_DEAD:
        break;
    case JAISoundStatus_::State_PREPARE:
        if (prepare_prepareStream_()) {
            mStatus.setPlaying();
            prepare_startStream_();
        }
        break;
    case JAISoundStatus_::State_LOCK_PREPARE:
        if (prepare_prepareStream_()) {
            mStatus.setReadyLocked();
        }
        break;
    case JAISoundStatus_::State_READY:
        mStatus.setPlaying();
        prepare_startStream_();
        break;
    case JAISoundStatus_::State_PLAYING:
        break;
    }
}

void JAIStream::prepare_startStream_() {
    if (inner_.aramStream.start()) {
        mIsStreamStarted = false;
        mIsPaused = false;
        mPrepareState = 4;
    }
}

void JAIStream::JAIStreamMgr_mixOut_(const JASSoundParams& inParams, JAISoundActivity activity) {
    bool isPaused;
    JASSoundParams outParams;
    mParams.mixOutAll(inParams, &outParams, (mStatus.isMute() || activity.isMute()) ? 0.0f : mFader.getIntensity());

    if (soundStrategy != nullptr) {
        soundStrategy->mix(this, outParams);
    }

    JASSoundParams* mixParams = &outParams;
    if (mAudible != nullptr && mAudience != nullptr) {
        for (int i = 0; i < mAudience->getMaxChannels(); i++) {
            JASSoundParams* outerParams = mAudible->getOuterParams(i);
            if (outerParams != nullptr) {
                mAudience->mixChannelOut(outParams, mAudible, i);
                mixParams = outerParams;
                break;
            }
        }
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        inner_.aramStream.setPitch(mixParams->mPitch);
        if (children[i] != nullptr) {
            inner_.aramStream.setChannelVolume(i, children[i]->mMove.mParams.mVolume * mixParams->mVolume);
            inner_.aramStream.setChannelPan(i, (children[i]->mMove.mParams.mPan + mixParams->mPan) - 0.5f);
            inner_.aramStream.setChannelFxmix(i, children[i]->mMove.mParams.mFxMix + mixParams->mFxMix);
            inner_.aramStream.setChannelDolby(i, children[i]->mMove.mParams.mDolby + mixParams->mDolby);
        } else {
            inner_.aramStream.setChannelVolume(i, mixParams->mVolume);
            inner_.aramStream.setChannelPan(i, mixParams->mPan);
            inner_.aramStream.setChannelFxmix(i, mixParams->mFxMix);
            inner_.aramStream.setChannelDolby(i, mixParams->mDolby);
        }
    }

    prepare_();

    if (mPrepareState == 4) {
        isPaused = mStatus.isPaused() || activity.isPaused();
        if (isPaused != mIsPaused) {
            inner_.aramStream.pause(isPaused);
            mIsPaused = isPaused;
        }
    }
}

void JAIStream::die_JAIStream_() {
    die_JAISound_();

    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (children[i] != nullptr) {
            delete children[i];
            children[i] = nullptr;
        }
    }

    if (soundStrategy != nullptr) {
        soundStrategyMgr->deleteStrategy(soundStrategy);
        soundStrategy = nullptr;
    }
}

bool JAIStream::JAISound_tryDie_() {
    if (mIsStreamStarted) {
        die_JAIStream_();
        return true;
    }

    switch (mPrepareState) {
    case 0:
    case 1:
        die_JAIStream_();
        return true;
    case 2:
    case 3:
        mPrepareState = 5;
        inner_.aramStream.cancel();
        break;
    case 4:
        mPrepareState = 6;
        inner_.aramStream.stop(10);
        break;
    }

    return false;
}

void JAIStream::JAIStreamMgr_calc_() {
    if (mIsStreamStarted) {
        mPrepareState = 0;
        stop_JAISound_();
    }

    if (calc_JAISound_()) {
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (children[i] != nullptr) {
                children[i]->calc();
            }
        }

        if (soundStrategy != nullptr) {
            soundStrategy->calc(this);
        }
    }
}

s32 JAIStream::getNumChild() const {
    return NUM_CHILDREN;
}

JAISoundChild* JAIStream::getChild(int index) {
    if (children[index] == nullptr) {
        children[index] = new JAISoundChild();
    }
    return children[index];
}

void JAIStream::releaseChild(int index) {
    if (children[index] != nullptr) {
        delete children[index];
        children[index] = nullptr;
    }
}

JASTrack* JAIStream::getTrack() {
    return nullptr;
}

JASTrack* JAIStream::getChildTrack(int param_0) {
    return nullptr;
}

JAIStream* JAIStream::asStream() {
    return this;
}

JAITempoMgr* JAIStream::getTempoMgr() {
    return nullptr;
}

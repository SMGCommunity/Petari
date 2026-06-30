#include "JSystem/JAudio2/JAISe.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAISeMgr.hpp"

JAISe::JAISe(JAISeMgr* SeMgr, JAISoundStrategyMgr< JAISe >* soundStrategyMgr, u32 priority) : JSULink< JAISe >(this) {
    inner_.mSoundStrategyMgr = soundStrategyMgr;
    inner_.seMgr = SeMgr;
    inner_.mProperPriority = priority;
}

void JAISe::mixOut_(const JASSoundParams& params) {
    inner_.track.setTempoRate(inner_.mTempoMgr.getTempo());
    JASSoundParams mixParams;
    mParams.mixOutAll(params, &mixParams, mFader.getIntensity());

    if (inner_.mSoundStrategy != nullptr) {
        inner_.mSoundStrategy->mix(this, mixParams);
    }

    if (mAudible != nullptr) {
        int maxChannels = mAudience->getMaxChannels();
        for (int i = 0; i < maxChannels; i++) {
            mAudience->mixChannelOut(mixParams, mAudible, i);
        }
    } else {
        inner_.mSoundParams = mixParams;
    }
}

void JAISe::stopTrack_() {
    switch (inner_.track.getStatus()) {
    case JASTrack::STATUS_FREE:
        break;
    case JASTrack::STATUS_RUN:
        inner_.track.stopSeq();
        break;
    case JASTrack::STATUS_STOPPED:
        break;
    }
}

void JAISe::startTrack_(const JASSoundParams& params) {
    if (inner_.track.getStatus() == JASTrack::STATUS_STOPPED) {
        inner_.track.init();
    }

    if (mAudible != nullptr) {
        initTrack_JAISound_(&inner_.track);
    } else {
        mParams.mixOutAll(params, &inner_.mSoundParams, mFader.getIntensity());
        inner_.track.setChannelMgrCount(1);
        inner_.track.assignExtBuffer(0, &inner_.mSoundParams);
    }

    inner_._26C = true;
    inner_.track.setSeqData(inner_.mSeqData.data, inner_.mSeqData.offset);
    inner_.track.startSeq();
}

void JAISe::JAISeCategoryMgr_mixOut_(bool param_0, const JASSoundParams& params, JAISoundActivity activity) {
    mixOut_(params);
    if (isStopping()) {
        stopTrack_();
        return;
    }

    if (param_0) {
        if (isPlaying()) {
            if (inner_._26C) {
                switch (inner_.track.getStatus()) {
                case JASTrack::STATUS_STOPPED:
                    if (comesBack()) {
                        startTrack_(params);
                    } else {
                        stop_JAISound_();
                    }
                    break;
                case JASTrack::STATUS_RUN:
                    inner_.track.pause(isPaused() || activity.isPaused());
                    inner_.track.mute(isMute() || activity.isMute());
                }
            } else {
                startTrack_(params);
            }
        }
        return;
    }

    if (comesBack()) {
        if (isPauseWhenOut()) {
            inner_.track.pause(true);
        } else {
            stopTrack_();
        }
    } else {
        stopTrack_();
        stop_JAISound_();
    }
}

void JAISe::JAISeCategoryMgr_calc_() {
    prepare_();

    if (calc_JAISound_()) {
        inner_.mTempoMgr.calc();
        if (inner_.mSoundStrategy != nullptr) {
            inner_.mSoundStrategy->calc(this);
        }
    }

    if (isStopping() || mPriority == -1) {
        inner_.mPriority = -1;
    } else {
        inner_.mPriority = inner_.mProperPriority + mPriority;
    }
}

bool JAISe::JAISound_tryDie_() {
    switch (inner_.track.getStatus()) {
    case JASTrack::STATUS_FREE:
    case JASTrack::STATUS_STOPPED:
        if (inner_.mSoundStrategy != nullptr) {
            inner_.mSoundStrategyMgr->deleteStrategy(inner_.mSoundStrategy);
            inner_.mSoundStrategy = nullptr;
        }
        die_JAISound_();
        return true;
    case JASTrack::STATUS_RUN:
        break;
    }
    return false;
}

void JAISe::JAISeMgr_startID_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience) {
    start_JAISound_(id, posPtr, audience);
    inner_._26C = false;
    if (inner_.mSoundStrategyMgr != nullptr) {
        inner_.mSoundStrategy = inner_.mSoundStrategyMgr->calc(id);
    } else {
        inner_.mSoundStrategy = nullptr;
    }
}

s32 JAISe::getNumChild() const {
    return 0;
}

JAISoundChild* JAISe::getChild(int param_0) {
    return nullptr;
}

void JAISe::releaseChild(int param_0) {
}

bool JAISe::prepare_getSeqData_() {
    switch (inner_.seMgr->getSeqDataMgr()->getSeqData(getID(), &inner_.mSeqData)) {
    case 0:
        mStatus.stop();
        return false;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    }

    return false;
}

void JAISe::prepare_() {
    switch (mStatus.getState()) {
    case JAISoundStatus_::State_DEAD:
        break;
    case JAISoundStatus_::State_STOP:
        stop_JAISound_();
        break;
    case JAISoundStatus_::State_PREPARE:
        if (prepare_getSeqData_()) {
            mStatus.setPlaying();
        }
        break;
    case JAISoundStatus_::State_LOCK_PREPARE:
        if (prepare_getSeqData_()) {
            mStatus.setReadyLocked();
        }
        break;
    case JAISoundStatus_::State_READY:
        mStatus.setPlaying();
        break;
    case JAISoundStatus_::State_LOCK_READY:
    case JAISoundStatus_::State_PLAYING:
        break;
    }
}

JASTrack* JAISe::getTrack() {
    return &inner_.track;
}

JASTrack* JAISe::getChildTrack(int param_0) {
    return inner_.track.getChild(param_0);
}

JAISe* JAISe::asSe() {
    return this;
}

JAITempoMgr* JAISe::getTempoMgr() {
    return &inner_.mTempoMgr;
}

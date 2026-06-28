#include "JSystem/JAudio2/JAISeq.hpp"
#include "JSystem/JAudio2/JAIAudience.hpp"
#include "JSystem/JAudio2/JAISeqMgr.hpp"
#include "JSystem/JAudio2/JAISoundChild.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"

namespace {

    static bool JASTrack_isFreeOrStopped(JASTrack* track) {
        u32 status = track->getStatus();
        return status == JASTrack::STATUS_FREE || status == JASTrack::STATUS_STOPPED;
    }

}  // namespace

JAISeq::JAISeq(JAISeqMgr* seqMgr, JAISoundStrategyMgr< JAISeq >* soundStrategyMgr) : JSULink< JAISeq >(this) {
    inner_.seqMgr = seqMgr;
    inner_.strategyMgr = soundStrategyMgr;
    inner_.mSoundParams.init();
    for (int i = 0; i < NUM_CHILDREN; i++) {
        inner_.mSoundChild[i] = nullptr;
    }
}

void JAISeq::JAISeqMgr_startID_(JAISoundID id, const TVec3f* posPtr, JAIAudience* audience, int category, int numTracks) {
    inner_.mCategory = category;
    reserveChildTracks_(numTracks);
    start_JAISound_(id, posPtr, audience);

    if (inner_.strategyMgr) {
        soundStrategy = inner_.strategyMgr->calc(id);
    } else {
        soundStrategy = nullptr;
    }
}

void JAISeq::playSeqData_(const JASSoundParams& params, JAISoundActivity activity) {
    inner_.outputTrack.setSeqData(inner_.mSeqData.data, inner_.mSeqData.offset);
    if (mAudible) {
        initTrack_JAISound_(&inner_.outputTrack);
    } else {
        inner_.outputTrack.setChannelMgrCount(1);
        inner_.outputTrack.assignExtBuffer(0, &inner_.mSoundParams);
    }

    mixOut_(params, activity);
    inner_.outputTrack.startSeq();
}

void JAISeq::reserveChildTracks_(int n) {
    // FIXME: regswap for the ages here
    for (int i = 0; i < 2; i++) {
        JASTrack* track = new JASTrack();
        if (track != nullptr) {
            track->setAutoDelete(true);
            inner_.outputTrack.connectChild(i, track);

            for (int j = 0; j < JASTrack::MAX_CHILDREN; j++) {
                if (j + i * JASTrack::MAX_CHILDREN < n) {
                    JASTrack* track2 = new JASTrack();
                    if (track2 != nullptr) {
                        track2->setAutoDelete(true);
                        track->connectChild(j, track2);
                    }
                }
            }
        }
    }
}

void JAISeq::releaseChildTracks_() {
    for (u32 i = 0; i < 2; i++) {
        JASTrack* track = inner_.outputTrack.getChild(i);
        if (track != nullptr) {
            for (u32 j = 0; j < JASTrack::MAX_CHILDREN; j++) {
                JASTrack* track2 = track->getChild(j);
                if (track2 != nullptr) {
                    delete track2;
                }
            }
            delete track;
        }
    }
}

bool JAISeq::prepare_getSeqData_() {
    if (getID().isAnonymous()) {
        return true;
    }

    JAISeqDataMgr* seqDataMgr = inner_.seqMgr->getSeqDataMgr();

    switch (seqDataMgr->getSeqData(getID(), &inner_.mSeqData)) {
    case 0:
        mStatus.stop();
        break;
    case 1:
        increasePrepareCount_JAISound_();
        return false;
    case 2:
        return true;
    }

    return false;
}

bool JAISeq::prepare_(const JASSoundParams& params, JAISoundActivity activity) {
    switch (mStatus.getState()) {
    case JAISoundStatus_::State_DEAD:
        break;
    case JAISoundStatus_::State_STOP:
        stop_JAISound_();
        break;
    case JAISoundStatus_::State_PREPARE:
        if (prepare_getSeqData_()) {
            mStatus.setPlaying();
            playSeqData_(params, activity);
            return true;
        }
        break;
    case JAISoundStatus_::State_LOCK_PREPARE:
        if (prepare_getSeqData_()) {
            mStatus.setReadyLocked();
        }
        return false;
    case JAISoundStatus_::State_READY:
        mStatus.setPlaying();
        playSeqData_(params, activity);
        return true;
    case JAISoundStatus_::State_PLAYING:
        return true;
    }

    return false;
}

void JAISeq::JAISeqMgr_calc_() {
    // FIXME: inline moment
    if (inner_.outputTrack.getStatus() == JASTrack::STATUS_STOPPED ||
        (inner_.outputTrack.getStatus() == JASTrack::STATUS_FREE && mStatus.isStopped())) {
        die_();
        return;
    }

    if (calc_JAISound_()) {
        inner_.mTempoMgr.calc();
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (inner_.mSoundChild[i]) {
                inner_.mSoundChild[i]->calc();
            }
        }
        if (soundStrategy) {
            soundStrategy->calc(this);
        }
    }
}

void JAISeq::die_() {
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (inner_.mSoundChild[i]) {
            delete inner_.mSoundChild[i];
            inner_.mSoundChild[i] = nullptr;
        }
    }

    if (soundStrategy) {
        inner_.strategyMgr->deleteStrategy(soundStrategy);
        soundStrategy = nullptr;
    }

    releaseChildTracks_();
    die_JAISound_();
}

bool JAISeq::JAISound_tryDie_() {
    if (JASTrack_isFreeOrStopped(&inner_.outputTrack)) {
        die_();
        return true;
    }

    mFader.forceOut();
    mStatus.mState.flags.flag1 = true;

    {
        JASCriticalSection cs;
        inner_.outputTrack.stopSeq();
    }

    return false;
}

void JAISeq::mixOut_(const JASSoundParams& params, JAISoundActivity activity) {
    inner_.outputTrack.setTempoRate(inner_.mTempoMgr.getTempo());
    inner_.outputTrack.mute(activity.isMute() || mStatus.isMute());
    inner_.outputTrack.pause(activity.isPaused() || mStatus.isPaused());

    JASSoundParams outParams;
    mParams.mixOutAll(params, &outParams, mFader.getIntensity());

    if (soundStrategy) {
        soundStrategy->mix(this, outParams);
    }

    if (mAudible) {
        int maxChannels = mAudience->getMaxChannels();
        for (int i = 0; i < maxChannels; i++) {
            mAudience->mixChannelOut(outParams, mAudible, i);
        }
    } else {
        inner_.mSoundParams = outParams;
        for (int i = 0; i < NUM_CHILDREN; i++) {
            if (inner_.mSoundChild[i]) {
                JASTrack* track = JAISeq::getChildTrack(i);
                if (track) {
                    inner_.mSoundChild[i]->mixOut(track);
                }
            }
        }
    }
}

void JAISeq::JAISeqMgr_mixOut_(const JASSoundParams& params, JAISoundActivity activity) {
    if (prepare_(params, activity) && inner_.outputTrack.getStatus() == JASTrack::STATUS_RUN) {
        mixOut_(params, activity);
    }
}

s32 JAISeq::getNumChild() const {
    return NUM_CHILDREN;
}

JAISoundChild* JAISeq::getChild(int index) {
    if (inner_.mSoundChild[index]) {
        return inner_.mSoundChild[index];
    }

    inner_.mSoundChild[index] = new JAISoundChild();
    if (!inner_.mSoundChild[index]) {
        return nullptr;
    }

    return inner_.mSoundChild[index];
}

void JAISeq::releaseChild(int index) {
    if (inner_.outputTrack.getStatus() == JASTrack::STATUS_RUN && inner_.mSoundChild[index]) {
        JASTrack* track = getChildTrack(index);
        if (track) {
            track->assignExtBuffer(0, nullptr);
        }
        delete inner_.mSoundChild[index];
        inner_.mSoundChild[index] = nullptr;
    }
}

JASTrack* JAISeq::getTrack() {
    return &inner_.outputTrack;
}

JASTrack* JAISeq::getChildTrack(int id) {
    u32 index = u32(id) >> 4;
    JASTrack* track = inner_.outputTrack.getChild(index);

    if (track != nullptr) {
        JASTrack* track2 = track->getChild(id - (index << 4));
        return track2;
    }

    return nullptr;
}

JAISeq* JAISeq::asSeq() {
    return this;
}

JAITempoMgr* JAISeq::getTempoMgr() {
    return &inner_.mTempoMgr;
}

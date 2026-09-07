#include "Game/AudioLib/AudBgm.hpp"
#include "Game/AudioLib/AudBgmMgr.hpp"
#include "Game/AudioLib/AudBgmSetting.hpp"
#include "Game/AudioLib/AudBgmVolumeController.hpp"
#include "Game/AudioLib/AudFader.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/RhythmLib/AudBgmTempoAdjuster.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"
#include "Game/RhythmLib/AudRhythmSeqParser.hpp"
#include "Game/RhythmLib/AudRhythmWrap.hpp"
#include <JSystem/JAudio2/JAISound.hpp>
#include <JSystem/JAudio2/JAISoundChild.hpp>
#include <JSystem/JAudio2/JAISoundInfo.hpp>
#include <JSystem/JAudio2/JASCriticalSection.hpp>

AudBgm::AudBgm() : mVolumeController() {
}

void AudBgm::resetAuxVolume() {
    if (mVolumeController == nullptr) {
        return;
    }
    mVolumeController->moveAuxVolume(1.0f, 0);
    mVolumeController->moveNoteFairyVolume(1.0f, 0);
}

AudSingleBgm::AudSingleBgm() : AudBgm(), mHandle(), mSoundID(-1) {
    init();
}

void AudSingleBgm::init() {
    mSoundID.setAnonymous();
    initTrackController();
}

void AudSingleBgm::movement() {
    updateTrackControl();
    if (!mHandle.isSoundAttached()) {
        resetAuxVolume();
        return;
    }

    if (mVolumeController != nullptr) {
        mHandle->getAuxiliary().moveVolume(mVolumeController->getVolume(), 0);
    }
}

JAISoundHandle* AudSingleBgm::start(u32 soundID, bool lock) {
    JAISoundID id = soundID;
    mSoundID = soundID;

    if (mHandle.isSoundAttached()) {
        mHandle->stop();
    }
    resetAuxVolume();

    if (id.getSectionID() == 0x2) {  // STM
        AudWrap::getSystem()->getStreamMgr().startSound(id, &mHandle, nullptr);
    } else {  // BGM or MBGM
        AudSoundInfo* info = AudWrap::getSoundInfo();
        AudWrap::getSystem()->getSeqMgr().startSound(id, &mHandle, nullptr);
        if (mHandle.isSoundAttached()) {
            startTrackControl();
            changeTrackMuteState(0, 0);
        }
    }

    if (mHandle.isSoundAttached()) {
        if (mVolumeController != nullptr) {
            mHandle->getAuxiliary().moveVolume(mVolumeController->getVolume(), 0);
        }
        if (lock) {
            mHandle->lockWhenPrepared();
        }
    }

    return &mHandle;
}

void AudSingleBgm::stop(u32 time) {
    if (!mHandle.isSoundAttached()) {
        return;
    }

    mHandle->stop(time);

    if (mSoundID.getSectionID() != 0x2) {  // not STM
        stopTrackControl();
    }
}

bool AudSingleBgm::isPreparedPlay() {
    if (mHandle.isSoundAttached()) {
        return mHandle->isPrepared();
    }
    return false;
}

void AudSingleBgm::playAfterPrepared() {
    if (mHandle.isSoundAttached()) {
        mHandle->unlockIfLocked();
    }
}

JAISoundHandle* AudSingleBgm::getRhythmHandle() {
    if (mHandle.isSoundAttached()) {
        if (mHandle->getID().getSectionID() == 0x1) {  //  BGM or MBGM
            return &mHandle;
        }
    }
    return nullptr;
}

bool AudSingleBgm::moveVolume(f32 volume, u32 time) {
    if (!mHandle.isSoundAttached()) {
        return false;
    }

    if (mVolumeController != nullptr) {
        mVolumeController->moveAuxVolume(volume, time);
        return true;
    }

    return false;
}

bool AudSingleBgm::moveVolumeForNoteFairy(f32 volume, u32 time) {
    if (!mHandle.isSoundAttached()) {
        return false;
    }

    if (mVolumeController != nullptr) {
        mVolumeController->moveNoteFairyVolume(volume, time);
        return true;
    }

    return false;
}

void AudSingleBgm::changeTrackMuteState(s32 track, s32 time) {
    if (mSoundID.getSectionID() == 0x2) {  // STM
        return;
    }

    AudSoundInfo* info = AudWrap::getSoundInfo();
    bool isRhythm = info->isRhythmBgm(mSoundID);

    const AudBgmSetting::MuteState* muteState = AudBgmSetting::getMuteState(mSoundID, track);
    if (muteState == nullptr) {
        for (s32 i = 0; i < mNumTracks; i++) {
            if (i != 0 || !isRhythm) {
                mTrackController[i].setMuteState(AudFader::FadeState_FadeIn, time, false);
            }
        }
        return;
    }

    for (s32 i = 0; i < mNumTracks; i++) {
        u8 state = muteState->mMuteStates[i / 2];
        if (i % 2 == 0) {
            state >>= 4;
        }

        bool autoMute = false;
        if ((state & 0x8) > 0) {
            autoMute = true;
        }

        mTrackController[i].setMuteState(state & 0x7, time, autoMute);
    }
}

void AudSingleBgm::initTrackController() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].mTrackNo = i;
    }
}

void AudSingleBgm::startTrackControl() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].start(&mHandle);
    }
}

void AudSingleBgm::stopTrackControl() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].stop();
    }
}

void AudSingleBgm::updateTrackControl() {
    if (mSoundID.getSectionID() == 0x2) {  // STM
        return;
    }

    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].update();
    }
}

AudMultiBgm::AudMultiBgm() : AudBgm(), mHandle(), mRhythmHandle(), _1F4(), mBgmId(-1), mIsLocked() {
    init();
}

void AudMultiBgm::init() {
    initTrackController();
    for (s32 i = 0; i < mNumFaders; i++) {
        mFader[i].set(1.0f, 0);
    }
}

JAISoundHandle* AudMultiBgm::start(u32 soundID, bool lock) {
    if (!isStopping()) {
        stop(0);
    }
    mIsLocked = lock;
    return prepare(soundID);
}

void AudMultiBgm::stop(u32 time) {
    if (mRhythmHandle.isSoundAttached()) {
        mRhythmHandle->stop(time);
    }

    if (mHandle.isSoundAttached()) {
        mHandle->stop(time);
    }

    _1F4 = 0;
    mBgmId = -1;
}

bool AudMultiBgm::isPreparedPlay() {
    return isPrepared();
}

void AudMultiBgm::playAfterPrepared() {
    mIsLocked = false;
}

void AudMultiBgm::sendToSyncStream() {
    if (!mRhythmHandle.isSoundAttached()) {
        return;
    }

    if (!mHandle.isSoundAttached()) {
        return;
    }

    AudRhythmMeSystem* meSystem = AudRhythmMeSystem::getInstance();
    s32 idx = mRhythmStrategy.mBgmIdx;
    if (idx >= 0) {
        AudBgmTempoAdjuster* adjuster = meSystem->getSeqParser(idx)->getTempoAdjuster();
        f32 beatMul = AudBgmSetting::getBeatMulForMultiBgm(mBgmId);
        u32 introBeats = AudBgmSetting::getIntroBeatsForMultiBgm(mBgmId);
        u32 loopBeats = AudBgmSetting::getLoopBeatsForMultiBgm(mBgmId);
        u32 loopStartSamples = AudBgmSetting::getLoopStartSamplesForMultiBgm(mBgmId);
        u32 loopEndSamples = AudBgmSetting::getLoopEndSamplesForMultiBgm(mBgmId);
        JAIStream* stream = mHandle->asStream();

        JASCriticalSection crit;
        adjuster->setInfo(beatMul, introBeats, loopBeats, loopStartSamples, loopEndSamples);
        adjuster->setStream(stream);
    }
}

void AudMultiBgm::rejectFromSyncStream() {
    AudRhythmMeSystem* meSystem = AudRhythmMeSystem::getInstance();
    s32 idx = mRhythmStrategy.mBgmIdx;
    if (idx >= 0) {
        JASCriticalSection crit;
        AudBgmTempoAdjuster* adjuster = meSystem->getSeqParser(idx)->getTempoAdjuster();
        adjuster->reject();
    }
}

void AudMultiBgm::movement() {
    updateSyncProcess();
    updateTrackControl();

    for (s32 i = 0; i < mNumFaders; i++) {
        mFader[i].update();
    }

    f32 volume = 1.0f;
    if (mVolumeController != nullptr) {
        volume = mVolumeController->getVolume();
    }

    if (!mHandle.isSoundAttached()) {
        if (!mRhythmHandle.isSoundAttached()) {
            resetAuxVolume();
            if (mVolumeController != nullptr) {
                for (s32 i = 0; i < mNumFaders; i++) {
                    mFader[i].set(1.0f, 0);
                }
            }
        }
    } else {
        f32 volA = volume * mFader[0].getVolume();
        f32 volB = volume * mFader[1].getVolume();
        setStreamVolume(volA, volB);
    }

    if (mRhythmHandle.isSoundAttached()) {
        mRhythmHandle->getAuxiliary().moveVolume(volume, 0);
    }
}

bool AudMultiBgm::moveVolume(f32 volume, u32 time) {
    if (!mHandle.isSoundAttached() && !mRhythmHandle.isSoundAttached()) {
        return false;
    }

    if (mVolumeController != nullptr) {
        mVolumeController->moveAuxVolume(volume, time);
        return true;
    }

    return false;
}

bool AudMultiBgm::moveVolumeForNoteFairy(f32 volume, u32 time) {
    if (!mHandle.isSoundAttached() && !mRhythmHandle.isSoundAttached()) {
        return false;
    }

    if (mVolumeController != nullptr) {
        mVolumeController->moveNoteFairyVolume(volume, time);
        return true;
    }

    return false;
}

void AudMultiBgm::changeTrackMuteState(s32 track, s32 time) {
    if (!mRhythmHandle.isSoundAttached()) {
        return;
    }

    JAISoundID soundID = mRhythmHandle->getID();

    if (soundID.getSectionID() == 0x2) {  // STM
        return;
    }

    AudSoundInfo* info = AudWrap::getSoundInfo();
    bool isRhythm = info->isRhythmBgm(soundID);

    const AudBgmSetting::MuteState* muteState = AudBgmSetting::getMuteState(soundID, track);
    if (muteState == nullptr) {
        for (s32 i = 0; i < mNumTracks; i++) {
            if (i != 0 || !isRhythm) {
                mTrackController[i].setMuteState(AudFader::FadeState_FadeIn, time, false);
            }
        }
        return;
    }

    switch (soundID) {
    case BGM_GALAXY_01:
        if (track == 1) {
            time *= 3;
        }
        break;
    }

    for (s32 i = 0; i < mNumTracks; i++) {
        u8 state = muteState->mMuteStates[i / 2];
        if (i % 2 == 0) {
            state >>= 4;
        }

        bool autoMute = false;
        if ((state & 0x8) > 0) {
            autoMute = true;
        }

        mTrackController[i].setMuteState(state & 0x7, time, autoMute);
    }

    u8 volA = muteState->mVolumeA;
    if (volA != 0xFF) {
        mFader[0].set(volA / 254.0f, time);
    }

    u8 volB = muteState->mVolumeB;
    if (volB != 0xFF) {
        mFader[1].set(volB / 254.0f, time);
    }

    if (track != 0) {
        u16 extraChordNum = AudBgmSetting::getExtraChordNum(soundID, track);
        if (extraChordNum != 0xFFFF) {
            AudWrap::getSystem()->getChordInfo()->loadChordInfo(extraChordNum, true);
        }
    }
}

JAISoundHandle* AudMultiBgm::getHandle() {
    return &mHandle;
}

JAISoundHandle* AudMultiBgm::getRhythmHandle() {
    return &mRhythmHandle;
}

bool AudMultiBgm::isSoundAttached() const {
    return mHandle.isSoundAttached();
}

void AudMultiBgm::pause(bool pause) {
    if (mHandle.isSoundAttached()) {
        mHandle->pause(pause);
    }
}

bool AudMultiBgm::isStopping() const {
    if (mHandle.isSoundAttached()) {
        return mHandle->isStopping();
    }
    return true;
}

bool AudMultiBgm::isPaused() const {
    if (mHandle.isSoundAttached()) {
        return mHandle->isPaused();
    }
    return false;
}

JAISoundID AudMultiBgm::getSoundID() const {
    if (!mHandle.isSoundAttached()) {
        return 0;
    }
    return mHandle->getID();
}

void AudMultiBgm::initTrackController() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].mTrackNo = i;
    }
}

void AudMultiBgm::startTrackControl() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].start(&mRhythmHandle);
    }
}

void AudMultiBgm::updateTrackControl() {
    for (s32 i = 0; i < mNumTracks; i++) {
        mTrackController[i].update();
    }
}

JAISoundHandle* AudMultiBgm::prepare(u32 id) {
    u32 bgmId = id & ~(0x01010000);

    u32 seqID = AudBgmSetting::getSeqIdForMultiBgm(bgmId);
    u32 streamID = AudBgmSetting::getStreamIdForMultiBgm(bgmId);

    AudWrap::getSystem()->getSeqMgr().startSound(seqID, &mRhythmHandle, nullptr);
    if (mRhythmHandle.isSoundAttached()) {
        mRhythmHandle->lockWhenPrepared();

        if (mVolumeController != nullptr) {
            mRhythmHandle->getAuxiliary().moveVolume(mVolumeController->getVolume(), 0);
        }

        startTrackControl();
        changeTrackMuteState(AudFader::FadeState_FadeOut, 0);
    } else {
        return nullptr;
    }

    AudWrap::getSystem()->getStreamMgr().startSound(streamID, &mHandle, nullptr);
    if (mHandle.isSoundAttached()) {
        mHandle->lockWhenPrepared();

        if (mVolumeController != nullptr) {
            f32 volA = mVolumeController->getVolume() * mFader[0].getVolume();
            f32 volB = mVolumeController->getVolume() * mFader[1].getVolume();
            setStreamVolume(volA, volB);
        }
    } else {
        mRhythmHandle->stop();
        return nullptr;
    }

    mBgmId = bgmId;
    _1F4 = 1;
    return &mHandle;
}

bool AudMultiBgm::isPrepared() {
    bool rhythmHandlePrepared = true;
    bool handlePrepared = false;

    if (mRhythmHandle.isSoundAttached()) {
        rhythmHandlePrepared = mRhythmHandle->isPrepared();
    }
    if (mHandle.isSoundAttached()) {
        handlePrepared = mHandle->isPrepared();
    }

    return rhythmHandlePrepared && handlePrepared;
}

void AudMultiBgm::unlock() {
    if (mIsLocked) {
        return;
    }

    if (mRhythmHandle.isSoundAttached()) {
        mRhythmHandle->unlockIfLocked();
    }
    if (mHandle.isSoundAttached()) {
        mHandle->unlockIfLocked();
    }

    _1F4 = 2;
}

void AudMultiBgm::updateSyncProcess() {
    pauseSyncProcess();

    if (_1F4 == 1) {
        if (isPrepared()) {
            unlock();
        }
    }
}

void AudMultiBgm::pauseSyncProcess() {
    bool isPaused = false;
    if (mHandle.isSoundAttached()) {
        isPaused = mHandle->asStream()->isPaused();
    }

    if (isPaused) {
        if (mRhythmHandle.isSoundAttached()) {
            mRhythmHandle->pause(true);
        }
    } else {
        if (mRhythmHandle.isSoundAttached()) {
            mRhythmHandle->pause(false);
        }
    }
}

void AudMultiBgm::setStreamVolume(f32 volA, f32 volB) {
    mHandle->getChild(0)->getMoveParams()->moveVolume(volA, 0);
    mHandle->getChild(1)->getMoveParams()->moveVolume(volA, 0);
    mHandle->getChild(2)->getMoveParams()->moveVolume(volB, 0);
    mHandle->getChild(3)->getMoveParams()->moveVolume(volB, 0);
}

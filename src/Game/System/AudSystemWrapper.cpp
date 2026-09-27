#include "Game/System/AudSystemWrapper.hpp"
#include "Game/AudioLib/AudMeNameConverter.hpp"
#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudSpeakerWrap.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/CSSoundNameConverter.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>

namespace AudRhythmWrap {
    void rhythmProc();
};  // namespace AudRhythmWrap

AudSystemWrapper::AudSystemWrapper(JKRSolidHeap* pSolidHeap, JKRHeap* pHeap) {
    volatile AudSystemWrapper* self = this;

    self->mIsResetDone = false;
    self->mSolidHeap = pSolidHeap;
    self->mAudSystem = nullptr;
    self->mHeap = pHeap;
    self->mSmrRes = nullptr;
    self->mJaiSeqRes = nullptr;
    self->mJaiChordRes = nullptr;
    self->mJaiMeRes = nullptr;
    self->mJaiRemixSeqRes = nullptr;
    self->mSpkRes = nullptr;
    self->_28 = false;
    self->mDisableReset = false;
    mSpkHeap = JKRExpHeap::create(AudSpeakerWrap::getRequiredHeapSize(), pSolidHeap, false);
}

void AudSystemWrapper::requestResourceForInitialize() {
    MR::loadAsyncToMainRAM(sAudioResFile, nullptr, mHeap, JKRDvdRipper::ALLOC_DIRECTION_BACKWARD);
    MR::mountAsyncArchive(sJaiSeqArc, mSolidHeap);
    MR::mountAsyncArchive(sJaiChordArc, mSolidHeap);
    MR::mountAsyncArchive(sJaiMeArc, mSolidHeap);
    MR::mountAsyncArchive(AudSpeakerWrap::getResName(), mSpkHeap);
    MR::mountAsyncArchive(sJaiRemixArc, mSolidHeap);
}

void AudSystemWrapper::createAudioSystem() {
    receiveResourceForInitialize();

    if (mIsResetDone) {
        OSSuspendThread(OSGetCurrentThread());
    }

    mDisableReset = true;
    mAudSystem = AudNewAudSystem(mSolidHeap, mSmrRes, mJaiSeqRes, mJaiChordRes, mJaiMeRes, mJaiRemixSeqRes);
    mDisableReset = false;

    MR::removeFileConsideringLanguage(sAudioResFile);
    mSmrRes = nullptr;

    mAudSystem->setSpeakerResource(mSpkRes);
    createSoundNameConverter();
    AudMicWrap::setMicEnv();
}

void AudSystemWrapper::createSoundNameConverter() {
    AudSingletonHolder< AudSoundNameConverter >::init();
    AudSingletonHolder< AudMeNameConverter >::init();
    AudSingletonHolder< CSSoundNameConverter >::init();
}

void AudSystemWrapper::updateRhythm() {
    if (mAudSystem == nullptr) {
        return;
    }

    AudRhythmWrap::rhythmProc();
}

void AudSystemWrapper::movement() {
    if (mAudSystem == nullptr) {
        return;
    }

    mAudSystem->frameWork();
}

void AudSystemWrapper::stopAllSound(u32 time) {
    mAudSystem->stop(time);
}

bool AudSystemWrapper::isLoadDoneWaveDataAtSystemInit() const {
    if (mAudSystem == nullptr) {
        return false;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return false;
    }

    return mAudSystem->mSceneMgr->isLoadDoneSystemInit();
}

void AudSystemWrapper::loadStaticWaveData() {
    if (mAudSystem == nullptr) {
        return;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return;
    }

    mAudSystem->mSceneMgr->loadStaticResource();
}

bool AudSystemWrapper::isLoadDoneStaticWaveData() const {
    if (mAudSystem == nullptr) {
        return false;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return false;
    }

    return mAudSystem->mSceneMgr->isLoadDoneStaticResource();
}

void AudSystemWrapper::loadStageWaveData(const char* pSceneName, const char* pStageName, bool isPlayerLuigi) {
    if (mAudSystem == nullptr) {
        return;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return;
    }

    if (isPlayerLuigi) {
        mAudSystem->mSceneMgr->setPlayerModeLuigi();
    } else {
        mAudSystem->mSceneMgr->setPlayerModeMario();
    }

    mAudSystem->mSceneMgr->loadStageResource(pSceneName, pStageName);
}

bool AudSystemWrapper::isLoadDoneStageWaveData() const {
    if (mAudSystem == nullptr) {
        return false;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return false;
    }

    return mAudSystem->mSceneMgr->isLoadDoneStageResource();
}

void AudSystemWrapper::loadScenarioWaveData(const char* pSceneName, const char* pStageName, s32 scenarioNo) {
    if (mAudSystem == nullptr) {
        return;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return;
    }

    mAudSystem->mSceneMgr->loadScenarioResource(pSceneName, pStageName, scenarioNo);
}

bool AudSystemWrapper::isLoadDoneScenarioWaveData() const {
    if (mAudSystem == nullptr) {
        return false;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return false;
    }

    return mAudSystem->mSceneMgr->isLoadDoneScenarioResource();
}

bool AudSystemWrapper::isPermitToReset() const {
    return !mDisableReset;
}

void AudSystemWrapper::prepareReset() {
    if (mAudSystem == nullptr) {
        mIsResetDone = true;
    } else {
        mAudSystem->preProcessToReset();
    }
}

void AudSystemWrapper::requestReset(bool stopThreads) {
    if (mAudSystem == nullptr) {
        mIsResetDone = true;
    } else {
        mAudSystem->resetAudio(10, stopThreads);
        mAudSystem->stop(10);
    }
}

bool AudSystemWrapper::isResetDone() {
    if (mIsResetDone) {
        return true;
    }

    if (mAudSystem == nullptr) {
        return true;
    }

    return mAudSystem->hasReset();
}

void AudSystemWrapper::resumeReset() {
    if (mIsResetDone) {
        mIsResetDone = false;
    }

    if (mAudSystem == nullptr) {
        return;
    }

    return mAudSystem->resumeReset();
}

void AudSystemWrapper::receiveResourceForInitialize() {
    mSmrRes = MR::receiveFile(sAudioResFile);
    mJaiSeqRes = MR::receiveArchive(sJaiSeqArc);
    mJaiChordRes = MR::receiveArchive(sJaiChordArc);
    mJaiMeRes = MR::receiveArchive(sJaiMeArc);
    mSpkRes = MR::receiveArchive(AudSpeakerWrap::getResName());
    mJaiRemixSeqRes = MR::receiveArchive(sJaiRemixArc);
}

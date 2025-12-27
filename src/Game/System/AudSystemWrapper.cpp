#include "Game/System/AudSystemWrapper.hpp"
#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudSpeakerWrap.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/RhythmLib/AudRhythmWrap.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/Util/FileUtil.hpp"
#include <JSystem/JKernel/JKRDvdRipper.hpp>
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRMemArchive.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>

AudSystemWrapper::AudSystemWrapper(JKRSolidHeap* pParam1, JKRHeap* pParam2) :
    mAudSystem(nullptr),
    _4(pParam1),
    _8(pParam2),
    mSmrRes(nullptr),
    mJaiSeqRes(nullptr),
    mJaiCordRes(nullptr),
    mJaiMeRes(nullptr),
    mJaiRemixSeqRes(nullptr),
    mSpkHeap(nullptr),
    mSpkRes(nullptr),
    _28(false),
    _29(false),
    _2A(false)
{
    mSpkHeap = JKRExpHeap::create(AudSpeakerWrap::getRequiredHeapSize(), pParam1, false);
}

void AudSystemWrapper::requestResourceForInitialize() {
    MR::loadAsyncToMainRAM("/AudioRes/SMR.szs", nullptr, _8, JKRDvdRipper::ALLOC_DIRECTION_2);
    MR::mountAsyncArchive("/AudioRes/Seqs/JaiSeq.arc", _4);
    MR::mountAsyncArchive("/AudioRes/Seqs/JaiChord.arc", _4);
    MR::mountAsyncArchive("/AudioRes/Seqs/JaiMe.arc", _4);
    MR::mountAsyncArchive(AudSpeakerWrap::getResName(), mSpkHeap);
    MR::mountAsyncArchive("/AudioRes/Info/JaiRemixSeq.arc", _4);
}

void AudSystemWrapper::createAudioSystem() {
    receiveResourceForInitialize();

    if (_29) {
        OSSuspendThread(OSGetCurrentThread());
    }

    _2A = true;
    mAudSystem = AudNewAudSystem(_4, mSmrRes, mJaiSeqRes, mJaiCordRes, mJaiMeRes, mJaiRemixSeqRes);
    _2A = false;

    MR::removeFileConsideringLanguage("/AudioRes/SMR.szs");
    mSmrRes = nullptr;

    mAudSystem->setSpeakerResource(mSpkRes);
    createSoundNameConverter();
    AudMicWrap::setMicEnv();
}
/*
void AudSystemWrapper::createSoundNameConverter() {
    AudSingletonHolder<AudSoundNameConverter>::init();
    AudSingletonHolder<AudMeNameConverter>::init();
    AudSingletonHolder<CSSoundNameConverter>::init();
}
*/
void AudSystemWrapper::updateRhythm() {
    if (mAudSystem == nullptr) {
        return;
    }

    AudRhythmWrap::rhythmProc();
}

// AudSystemWrapper::movement
// AudSystemWrapper::stopAllSound

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

void AudSystemWrapper::loadStageWaveData(const char* pParam1, const char* pParam2, bool isPlayerLuigi) {
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

    mAudSystem->mSceneMgr->loadStageResource(pParam1, pParam2);
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

void AudSystemWrapper::loadScenarioWaveData(const char* pParam1, const char* pParam2, s32 param3) {
    if (mAudSystem == nullptr) {
        return;
    }

    if (mAudSystem->mSceneMgr == nullptr) {
        return;
    }

    mAudSystem->mSceneMgr->loadScenarioResource(pParam1, pParam2, param3);
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
    return !_2A;
}

void AudSystemWrapper::prepareReset() {
    if (mAudSystem == nullptr) {
        _29 = true;
    } else {
        mAudSystem->preProcessToReset();
    }
}

void AudSystemWrapper::requestReset(bool param1) {
    if (mAudSystem == nullptr) {
        _29 = true;
    } else {
        mAudSystem->resetAudio(10, param1);
        // mAudSystem->stop(10);
    }
}

bool AudSystemWrapper::isResetDone() {
    if (_29) {
        return true;
    }

    if (mAudSystem == nullptr) {
        return true;
    }

    return mAudSystem->hasReset();
}

void AudSystemWrapper::resumeReset() {
    if (_29) {
        _29 = false;
    }

    if (mAudSystem == nullptr) {
        return;
    }

    return mAudSystem->resumeReset();
}

void AudSystemWrapper::receiveResourceForInitialize() {
    mSmrRes = MR::receiveFile("/AudioRes/SMR.szs");
    mJaiSeqRes = MR::receiveArchive("/AudioRes/Seqs/JaiSeq.arc");
    mJaiCordRes = MR::receiveArchive("/AudioRes/Seqs/JaiChord.arc");
    mJaiMeRes = MR::receiveArchive("/AudioRes/Seqs/JaiMe.arc");
    mSpkRes = MR::receiveArchive(AudSpeakerWrap::getResName());
    mJaiRemixSeqRes = MR::receiveArchive("/AudioRes/Info/JaiRemixSeq.arc");
}

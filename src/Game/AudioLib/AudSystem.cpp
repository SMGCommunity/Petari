#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudEffector.hpp"
#include "Game/AudioLib/AudLimitedSound.hpp"
#include "Game/AudioLib/AudParams.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudSeStrategy.hpp"
#include "Game/AudioLib/AudSoundId.hpp"
#include "Game/AudioLib/AudSoundInfo.hpp"
#include "Game/AudioLib/AudSoundObjHolder.hpp"
#include "Game/AudioLib/AudSoundObject.hpp"
#include "Game/AudioLib/AudSystemVolumeController.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudMeChannelMgr.hpp"
#include "Game/RhythmLib/AudMeObject.hpp"
#include "Game/RhythmLib/AudMePlayer.hpp"
#include "Game/RhythmLib/AudMeTrack.hpp"
#include "Game/RhythmLib/AudRhythmMeSystem.hpp"
#include "Game/Speaker/SpkSystem.hpp"
#include <JSystem/JAudio2/JAIAudible.hpp>
#include <JSystem/JAudio2/JAISeMgr.hpp>
#include <JSystem/JAudio2/JAISeqDataMgr.hpp>
#include <JSystem/JAudio2/JAISoundStarter.hpp>
#include <JSystem/JAudio2/JAIStreamDataMgr.hpp>
#include <JSystem/JAudio2/JASAiCtrl.hpp>
#include <JSystem/JAudio2/JASAramStream.hpp>
#include <JSystem/JAudio2/JASCriticalSection.hpp>
#include <JSystem/JAudio2/JASDriverIF.hpp>
#include <JSystem/JAudio2/JASSoundParams.hpp>
#include <JSystem/JAudio2/JAUAudioArcLoader.hpp>
#include <JSystem/JAudio2/JAUInitializer.hpp>
#include <JSystem/JAudio2/JAUSectionHeap.hpp>
#include <JSystem/JAudio2/JAUSoundMgr.hpp>
#include <JSystem/JAudio2/JAUStdSoundInfo.hpp>
#include <JSystem/JAudio2/JAUStreamDataMgr.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include <JSystem/JKernel/JKRArchive.hpp>
#include <JSystem/JKernel/JKRHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <revolution/sc.h>
#include <revolution/vi/vifuncs.h>

AudSystem* AudNewAudSystem_(JAUSectionHeap* pHeap, JKRArchive* pChordArchive, JKRArchive* pMeArchive, JKRArchive* pRemixArchive, int maxChannels) {
    AudSystem* system = new AudSystem(pHeap, pChordArchive, pMeArchive, pRemixArchive);
    system->mAudience.mMaxChannels = maxChannels;
    return system;
}

namespace {
    int init_JAS_JAI_(JKRSolidHeap* pHeap) NO_INLINE {
        JAU_JASInitializer initJAS;
        initJAS.audioMemory_ = AudParams::aramAddr;
        initJAS.audioMemSize_ = AudParams::aramSize;
        initJAS.heapSize_ = AudParams::workAreaSize;
        initJAS.dvdThreadPriority_ = AudParams::dvdThreadPriority;
        initJAS.audioThreadPriority_ = AudParams::audioThreadPriority;
        initJAS.dvdThreadId_ = AudParams::dvdThreadSwitchID;
        initJAS.audioThreadId_ = AudParams::audioThreadSwitchID;
        initJAS.field_0x1c = AudParams::numFreeTracks;
        initJAS.field_0x20 = AudParams::numFreeTrackChannels;
        initJAS.aramBlockSize_ = AudParams::streamBlockSize;
        initJAS.aramChannelNum_ = AudParams::numStreamChannels;
        initJAS.dspLevel_ = AudParams::outputGain;
        initJAS.initJASystem(pHeap);

        JAU_JAIInitializer initJAI;
        initJAI.mMaxNumSe = AudParams::numFreeSe;
        initJAI.mMaxNumSeq = AudParams::numFreeSeq;
        initJAI.mMaxNumStream = AudParams::numFreeStream;
        initJAI.mMaxNumChildren = AudParams::numFreeSoundChild;
        initJAI.initJAInterface();

        AudDopplerAudible< 4 >::newMemPool(initJAI.mMaxNumSe);
        AudSeStrategy::newMemPool(initJAI.mMaxNumSe);
        SpkSystem::newSoundMemPool(64);
        AudMe::newMemPool(32);       // FIXME: size
        AudMeTrack::newMemPool(96);  // FIXME: size

        return initJAI.mMaxNumSeq;
    }
};  // namespace

AudSystem* AudNewAudSystem(JKRSolidHeap* pHeap, void* pV, JKRArchive* pSeqArchive, JKRArchive* pChordArchive, JKRArchive* pMeArchive,
                           JKRArchive* pRemixArchive) {
    JKRHeap* currHeap = JKRGetCurrentHeap();
    int maxNumSeq = ::init_JAS_JAI_(pHeap);
    JAUSectionHeap* newHeap = JAUNewSectionHeap(true);
    newHeap->getHeap()->becomeCurrentHeap();

    if (pV == nullptr) {
        if (DVDConvertPathToEntrynum("/AudioRes/SMR.szs") >= 0) {
            JAUAudioArcLoader loader(newHeap);
            loader.load("/AudioRes/SMR.szs");
            newHeap->getHeap()->freeTail();
        }
    } else {
        JAUAudioArcLoader loader(newHeap);
        loader.load(pV);
        newHeap->getHeap()->freeTail();
    }

    u32 maxSeqDataSize;
    if (pSeqArchive == nullptr) {
        if (DVDConvertPathToEntrynum("/AudioRes/Seqs/JaiSeq.arc") >= 0) {
            JKRArchive* seqArc =
                JKRArchive::mount("/AudioRes/Seqs/JaiSeq.arc", JKRArchive::MOUNT_MODE_DVD, newHeap->getHeap(), JKRArchive::MOUNT_DIRECTION_1);
            newHeap->setSeqDataArchive(seqArc);
            maxSeqDataSize = AudParams::maxSeqDataSize;
        } else {
            maxNumSeq = 0;
        }
    } else {
        newHeap->setSeqDataArchive(pSeqArchive);
        maxSeqDataSize = AudParams::maxSeqDataSize;
    }

    if (pChordArchive == nullptr) {
        pChordArchive =
            JKRArchive::mount("/AudioRes/Info/JaiChord.arc", JKRArchive::MOUNT_MODE_MEM, newHeap->getHeap(), JKRArchive::MOUNT_DIRECTION_1);
    }

    if (pMeArchive == nullptr) {
        pMeArchive = JKRArchive::mount("/AudioRes/Info/JaiMe.arc", JKRArchive::MOUNT_MODE_MEM, newHeap->getHeap(), JKRArchive::MOUNT_DIRECTION_1);
    }

    AudSystem* system = AudNewAudSystem_(newHeap, pChordArchive, pMeArchive, pRemixArchive, 1);
    if (maxSeqDataSize != 0) {
        newHeap->newDynamicSeqBlocks(maxSeqDataSize, maxNumSeq);
    }

    currHeap->becomeCurrentHeap();
    newHeap->finishBuild();

    return system;
}

AudSystem::AudSystem(JAUSectionHeap* pSectionHeap, JKRArchive* pChordArchive, JKRArchive* pMeArchive, JKRArchive* pRemixArchive)
    : mIsResetReady(), mIsReset(), mStopThreads(), _828(), mIsPaused(), _82A(), _82B(), _82C(), _830(-1), mSoundInfo(), mSectionHeap(pSectionHeap),
      mStreamAramMgr(), mAudible(), mSystemSeObject(), mSystemMeObject(), mSceneMgr(), mRhythmMeSystem(), mRemixMgr(), mAudEffector(),
      mSeStrategyMgr(), mVolumeController(), mHomeButtonMenuState(), mHomeButtonToggleTime(-1), mPauseMenuState(), mPauseMenuToggleTime(-1),
      mIsDvdError(), mLimitedSoundInfo(), mSpkSystem() {
    // FIXME: inline chains and ctors.
    msBasic = this;
    mLimitedSoundInfo = new (mSectionHeap->getHeap(), 0) AudLimitedSoundInfo[2];
    clearAllLimitedSound();

    JAISeqDataMgr* seSeqDataMgr = pSectionHeap->getSeSeqDataMgr();
    if (seSeqDataMgr != nullptr) {
        getSeMgr().setSeqDataMgr(seSeqDataMgr);
    }

    JAISeqDataMgr* seqSeqDataMgr = pSectionHeap->getSeqSeqDataMgr();
    if (seqSeqDataMgr != nullptr) {
        getSeqMgr().setSeqDataMgr(seqSeqDataMgr);
    }

    setAudience(&mAudience);

    /*   FIXME    */
    JAUStreamStaticAramMgr* streamAramMgr = new JAUStreamStaticAramMgr();
    JASHeap* aramHeap = JASKernel::getAramHeap();
    streamAramMgr->alloc(aramHeap, JASAramStream::getBlockSize() * AudParams::streamAramBlocks_perChunk);
    mStreamAramMgr = streamAramMgr;
    mStreamMgr.setStreamAramMgr(mStreamAramMgr);
    /*   -----    */

    mSoundInfo = nullptr;
    JAUSoundTable* soundTable = mSectionHeap->getSoundTable();
    if (soundTable != nullptr) {
        JAUSectionHeap* heap = mSectionHeap->getSectionHeap();
        JKRHeap* prevHeap = heap->mHeap->becomeCurrentHeap();

        /*   FIXME    */
        JAUDisposerObject_< JAUStdSoundInfo >* soundInfo = new JAUDisposerObject_< JAUStdSoundInfo >(true);
        /*   -----    */

        heap->get_C4().append(soundInfo);
        prevHeap->becomeCurrentHeap();
        mSoundInfo = soundInfo;
    }

    JAIStreamDataMgr* streamDataMgr = pSectionHeap->getStreamDataMgr();
    if (streamDataMgr != nullptr) {
        mStreamMgr.setStreamDataMgr(streamDataMgr);
    } else if (mSoundInfo != nullptr) {
        mStreamMgr.setStreamDataMgr(mSoundInfo);
    }

    initCategoryArrangement();
    initVolumeSetting();

    mSoundObjHolder = new (mSectionHeap->getHeap(), 0) AudSoundObjHolder(mSectionHeap->getHeap(), AudParams::numInspectableSoundObj);
    mSystemSeObject = new (mSectionHeap->getHeap(), 0) AudSoundObject(nullptr, 10, mSectionHeap->getHeap());
    mAtmosphereSeObject = new (mSectionHeap->getHeap(), 0) AudSoundObject(nullptr, 8, mSectionHeap->getHeap());
    mSceneMgr = new (mSectionHeap->getHeap(), 0) AudSceneMgr(mSectionHeap);
    mRhythmMeSystem = new (mSectionHeap->getHeap(), 0) AudRhythmMeSystem(mSectionHeap->getHeap(), 3, true);
    mRemixMgr = new (mSectionHeap->getHeap(), 0) AudRemixMgr(mSectionHeap->getHeap());
    mRemixMgr->init();
    mSeStrategyMgr = new (mSectionHeap->getHeap(), 0) AudSeStrategyMgr();
    mSeMgr.setStrategyMgr(mSeStrategyMgr);
    mAudEffector = new (mSectionHeap->getHeap(), 0) AudEffector();
    mAudEffector->initFxLine(mSectionHeap->getHeap());

    if (pChordArchive != nullptr) {
        mRhythmMeSystem->mChordInfo.init(pChordArchive);
    }
    if (pMeArchive != nullptr) {
        setMeResource(pMeArchive, 1, 0);
    }
    if (pRemixArchive != nullptr) {
        mRemixMgr->setRemixSeqResource(pRemixArchive->getResource(static_cast< u16 >(0)));
    }

    mSystemMeObject = new (mSectionHeap->getHeap(), 0) AudMeObject(nullptr, 8, mSectionHeap->getHeap());
    mSpkSystem = new (mSectionHeap->getHeap(), 0) SpkSystem(mSectionHeap->getHeap());
    mVolumeController = new (mSectionHeap->getHeap(), 0) AudSystemVolumeController(this);
}

AudChordInfo* AudSystem::getChordInfo() {
    return &mRhythmMeSystem->mChordInfo;
}

void AudSystem::setSpeakerResource(JKRArchive* pArchive) {
    mSpkSystem->setResource(pArchive, 0, 1);
}

void AudSystem::frameWork() {
    screenSaverProcess();
    updateHomeButtonMenu();
    updatePauseMenu();
    seFanfareProcess();

    if (mVolumeController != nullptr) {
        mVolumeController->update();
    }

    mBgmMgr.movement();
    mRhythmMeSystem->update();
    mRemixMgr->update();
    JAUSoundMgr::calc();
    mixOut();
    mSoundObjHolder->update();

    if (mSpkSystem != nullptr) {
        mSpkSystem->framework();
    }

    if (_830 >= 0) {
        _830--;
        if (_830 <= 0) {
            _830 = -1;
        }
    }

    updateLimitedSound();
}

void AudSystem::calc() {
    if (mIsReset && mReseter.checkDone()) {
        if (!mStopThreads) {
            stopSync();
        }
        return;
    }
    JAUSoundMgr::calc();
}

bool AudSystem::startSound(JAISoundID soundID, JAISoundHandle* pHandle, const TVec3f* pPos) {
    if (mIsResetReady && soundID.getGroupID() != 0xD) {  // HOMESE
        return false;
    }

    if (mIsReset) {
        return false;
    }

    if (_828) {
        return false;
    }

    if (_82A) {
        return false;
    }

    if (_830 >= 0 && soundID.getGroupID() != 0xD && soundID.getGroupID() != 0x0) {  // HOMESE, SE_SY
        return false;
    }

    if (_82B && soundID.getGroupID() != 0xD && soundID.getGroupID() != 0x0) {  // HOMESE, SE_SY
        return false;
    }

    if (isSubmitSeByVolumeSetting(soundID)) {
        return false;
    }

    if (soundID.getGroupID() != 0xD && mHomeButtonMenuState != 0) {  // disable non-home-menu sounds if menu is not cleared
        return false;
    }

    if (!JAUSoundMgr::startSound(soundID, pHandle, pPos)) {
        return false;
    }

    switchToLuigi(soundID, pHandle);
    return true;
}

bool AudSystem::startLevelSound(JAISoundID soundID, JAISoundHandle* pHandle, const TVec3f* pPos) {
    if (mIsResetReady && soundID.getGroupID() != 0xD) {  // HOMESE
        return false;
    }

    if (mIsReset) {
        return false;
    }

    if (_828) {
        return false;
    }

    if (_82A) {
        return false;
    }

    if (_830 >= 0 && soundID.getGroupID() != 0xD && soundID.getGroupID() != 0x0) {  // HOMESE, SE_SY
        return false;
    }

    if (_82C && soundID.getGroupID() != 0xD && soundID.getGroupID() != 0x0) {  // HOMESE, SE_SY
        return false;
    }

    if (isSubmitSeByVolumeSetting(soundID)) {
        return false;
    }

    if (soundID.getGroupID() != 0xD && mHomeButtonMenuState != 0) {  // disable non-home-menu sounds if menu is not cleared
        return false;
    }

    if (!JAISoundStarter::startLevelSound(soundID, pHandle, pPos)) {
        return false;
    }

    return true;
}

void AudSystem::switchToLuigi(JAISoundID soundID, JAISoundHandle* pHandle) {
    if (!mSceneMgr->isPlayerModeLuigi()) {
        return;
    }

    if (pHandle == nullptr) {
        return;
    }

    if (!pHandle->isSoundAttached()) {
        return;
    }

    if (soundID.getGroupID() != 0x1) {  // SE_PV
        return;
    }

    JASCriticalSection crit;

    JASTrack* track = pHandle->mSound->getTrack();
    if (track != nullptr) {
        track->writePort(12, 20);
    }
}

bool AudSystem::isSubmitSeByVolumeSetting(JAISoundID soundID) {
    if (mVolumeController == nullptr) {
        return false;
    }

    if (mVolumeController->mCurrVolumeSet >= 3 || mVolumeController->mCurrVolumeSet < 1) {
        f32 f1 = 0.0f;  // FIXME: DUMMY
    } else {
        switch (soundID) {
        case SE_SY_TIMER_A_2:
        case SE_SY_TIMER_A_1:
        case SE_SY_TIMER_A_0:
            return true;
        }

        return false;
    }

    return false;
}

void AudSystem::updateOutputMode() {
    switch (SCGetSoundMode()) {
    case 0:
        JASDriver::setOutputMode(0);  // neither? (TODO)
        break;
    case 1:
        JASDriver::setOutputMode(1);  // only pan
        break;
    case 2:
        JASDriver::setOutputMode(2);  // pan and effect
        break;
    }
}

void AudSystem::preProcessToReset() {
    mSeqMgr.stop(0);
    mStreamMgr.stop(0);
    mBgmMgr.clearLastBGM(AudBgmMgr::BgmType_Stage);
    mBgmMgr.clearLastBGM(AudBgmMgr::BgmType_Sub);
    mBgmMgr.clearNextBGM(AudBgmMgr::BgmType_Stage);
    mBgmMgr.clearNextBGM(AudBgmMgr::BgmType_Sub);

    if (mRhythmMeSystem != nullptr) {
        mRhythmMeSystem->mMeMgr->stopAll(-2);
    }

    mIsResetReady = true;
}

void AudSystem::resetAudio(u32 time, bool stopThreads) {
    mReseter.start(time * JASDriver::getSubFrames(), stopThreads);
    mStopThreads = stopThreads;
    mIsReset = true;
}

void AudSystem::resumeReset() {
    mReseter.resume();
    initParams();
    initVolumeSetting();
    if (mVolumeController != nullptr) {
        mVolumeController->init();
    }
    _82A = false;
    _82B = false;
    _82C = false;
    mHomeButtonMenuState = MenuState_Off;
    mHomeButtonToggleTime = -1;
    mPauseMenuState = MenuState_Off;
    mPauseMenuToggleTime = -1;
    mIsReset = false;
    mIsResetReady = false;
}

bool AudSystem::hasReset() const {
    return mIsReset && mReseter.checkDone() && (mStopThreads || getNumActiveSounds() == 0);
}

void AudSystem::initSceneVolume() {
    unpause();
    if (mVolumeController != nullptr) {
        mVolumeController->init();
    }
    setSeVolumeSet(AudSystemVolumeController::VolumeSet_0, 0);
    mSeqMgr.getParams()->moveVolume(AudParams::masterVolBgm, 0);
    mStreamMgr.getParams()->moveVolume(AudParams::masterVolStream, 0);
    mPauseMenuState = MenuState_Off;
    mPauseMenuToggleTime = -1;
}

void AudSystem::pause() {
    if (mIsPaused) {
        return;
    }

    mBgmMgr.pause();
    mRhythmMeSystem->setIdle(true);
    mIsPaused = true;
}

void AudSystem::unpause() {
    mBgmMgr.unpause();
    mRhythmMeSystem->setIdle(false);
    mIsPaused = false;
}

void AudSystem::enterHomeButtonMenu() {
    mHomeButtonMenuState = MenuState_Enter;
    mHomeButtonToggleTime = 18 + 1;
    mSeqMgr.getParams()->moveVolume(0.0f, 18);
    mStreamMgr.getParams()->moveVolume(0.0f, 18);
    setVolumeZeroForce(MenuType_HomeButtonMenu);
    setSeVolumeSet(AudSystemVolumeController::VolumeSet_HomeButtonMenu, 18);
}

void AudSystem::exitHomeButtonMenu() {
    updateOutputMode();
    mHomeButtonMenuState = MenuState_Exit;
    mHomeButtonToggleTime = 20 + 1;

    if (!isPauseMenuActive()) {
        mSeqMgr.getParams()->moveVolume(AudParams::masterVolBgm, 20);
        mStreamMgr.getParams()->moveVolume(AudParams::masterVolStream, 18);
    }

    recoverSeVolumeSet(20);

    if (!isPauseMenuActive() && !mIsDvdError) {
        unpause();
    }
}

void AudSystem::updateHomeButtonMenu() {
    switch (mHomeButtonMenuState) {
    case MenuState_Off:
        break;
    case MenuState_Enter:
        if (mHomeButtonToggleTime > 0 && --mHomeButtonToggleTime <= 0) {
            mHomeButtonToggleTime = -1;
            mHomeButtonMenuState = MenuState_Active;
            if (!isPauseMenuActive()) {
                pause();
            }
        }
        break;
    case MenuState_Exit:
        if (mHomeButtonToggleTime > 0 && --mHomeButtonToggleTime <= 0) {
            mHomeButtonToggleTime = -1;
            mHomeButtonMenuState = MenuState_Off;
        }
        break;
    case MenuState_Active:
        break;
    }
}

bool AudSystem::isHomeButtonMenuActive() const {
    if (mHomeButtonMenuState == MenuState_Off || mHomeButtonMenuState == MenuState_Exit) {
        return false;
    }
    return true;
}

void AudSystem::enterPauseMenu() {
    mPauseMenuState = MenuState_Enter;
    mPauseMenuToggleTime = 18 + 1;
    mSeqMgr.getParams()->moveVolume(0.0f, 18);
    mStreamMgr.getParams()->moveVolume(0.0f, 18);
    setVolumeZeroForce(MenuType_Pause);
    setSeVolumeSet(AudSystemVolumeController::VolumeSet_PauseMenu, 18);
}

void AudSystem::exitPauseMenu() {
    mPauseMenuState = MenuState_Exit;
    mPauseMenuToggleTime = 20 + 1;
    mSeqMgr.getParams()->moveVolume(AudParams::masterVolBgm, 20);
    mStreamMgr.getParams()->moveVolume(AudParams::masterVolStream, 20);
    recoverSeVolumeSet(20);
    unpause();
}

void AudSystem::updatePauseMenu() {
    switch (mPauseMenuState) {
    case MenuState_Off:
        break;
    case MenuState_Enter:
        if (mPauseMenuToggleTime > 0 && --mPauseMenuToggleTime <= 0) {
            mPauseMenuToggleTime = -1;
            mPauseMenuState = MenuState_Active;
            pause();
        }
        break;
    case MenuState_Exit:
        if (mPauseMenuToggleTime > 0 && --mPauseMenuToggleTime <= 0) {
            mPauseMenuToggleTime = -1;
            mPauseMenuState = MenuState_Off;
        }
        break;
    case MenuState_Active:
        break;
    }
}

bool AudSystem::isPauseMenuActive() const {
    if (mPauseMenuState == MenuState_Off || mPauseMenuState == MenuState_Exit) {
        return false;
    }
    return true;
}

void AudSystem::screenSaverProcess() {
    if (VIGetDimmingCount() > 0) {
        return;
    }
    setSeVolumeSetLevel(AudSystemVolumeController::VolumeSet_ScreenSaver);
    mBgmMgr.volDownLevel(false);
}

bool AudSystem::isEnableStartSound(JAISoundID soundID) {
    if (mIsReset) {
        return false;
    }

    if (_828) {
        return false;
    }

    if (mIsResetReady && soundID.getGroupID() != 0xD) {  // HOMESE
        return false;
    }

    if (_830 >= 0 && soundID.getGroupID() != 0xD && soundID.getGroupID() != 0x0) {  // HOMESE, SE_SY
        return false;
    }

    return true;
}

void AudSystem::doDvdErrorProcess() {
    mIsDvdError = true;
    setVolumeZeroForce(MenuType_Error);
    mSeMgr.stop(10);
    pause();
}

void AudSystem::exitDvdErrorProcess() {
    if (!isPauseMenuActive() && !isHomeButtonMenuActive()) {
        unpause();
    }
    mIsDvdError = false;
}

s32 AudSystem::getNumOfPlaying(JAISoundID soundID) {
    s32 count = 0;
    for (s32 category = 0; category < JAISeMgr::NUM_CATEGORIES; category++) {
        for (JSULink< JAISe >* it = getSeMgr().getCategory(category)->getSeList()->getFirst(); it != getSeMgr().getCategory(0)->getSeList()->getEnd();
             it = it->getNext()) {
            if (it->getObject()->mSoundID == soundID) {
                count++;
            }
        }
    }
    return count;
}

u32 AudSystem::getPlayCountMin(JAISoundID soundID) {
    u32 min = -1;
    for (s32 category = 0; category < JAISeMgr::NUM_CATEGORIES; category++) {
        for (JSULink< JAISe >* it = getSeMgr().getCategory(category)->getSeList()->getFirst(); it != getSeMgr().getCategory(0)->getSeList()->getEnd();
             it = it->getNext()) {
            if (it->getObject()->mSoundID == soundID) {
                if (it->getObject()->getCount() < min) {
                    min = it->getObject()->getCount();
                }
            }
        }
    }
    return min;
}

void AudSystem::setMicMtx(MtxPtr pMtx, s32 mic) {
    mAudience.mStates[mic].setMtx(pMtx);
}

const TVec3f& AudSystem::getMicPos(s32 mic) {
    return mAudience.mStates[mic].getPos();
}

void AudSystem::setFarCamera(bool isFar) {
    mAudience.mFarCamera = isFar;
}

void AudSystem::setSeVolumeSet(s32 volumeSet, u32 maxSteps) {
    if (mVolumeController != nullptr) {
        mVolumeController->setSeVolumeSetTrig(volumeSet, maxSteps);
    }
}

void AudSystem::recoverSeVolumeSet(u32 maxSteps) {
    if (mVolumeController != nullptr) {
        mVolumeController->recoverSeVolumeSet(maxSteps);
    }
}

void AudSystem::setSeVolumeSetLevel(s32 volumeSet) {
    if (mVolumeController != nullptr) {
        mVolumeController->setSeVolumeSetLevel(volumeSet);
    }
}

void AudSystem::initVolumeSetting() {
    setSeVolumeSet(AudSystemVolumeController::VolumeSet_0, 0);
    mSeMgr.getParams()->moveVolume(AudParams::masterVolSe, 0);
    mSeqMgr.getParams()->moveVolume(AudParams::masterVolBgm, 0);
    mStreamMgr.getParams()->moveVolume(AudParams::masterVolStream, 0);
    if (mRhythmMeSystem != nullptr) {
        mRhythmMeSystem->mMeMgr->_50 = 1.0f;
    }
}

void AudSystem::initCategoryArrangement() {
    // FIXME: struct shouldnt be cleared beforehand!
    mSeMgr.setCategoryArrangement((JAISeCategoryArrangement){
        AudParams::arrangeSE_SY,
        AudParams::arrangeSE_PV,
        AudParams::arrangeSE_PM,
        AudParams::arrangeSE_BV,
        AudParams::arrangeSE_BM,
        AudParams::arrangeSE_OJ,
        AudParams::arrangeSE_AT,
        AudParams::arrangeSE_DM,
        AudParams::arrangeSE_EV,
        AudParams::arrangeSE_EM,
        AudParams::arrangeSE_SV,
        AudParams::arrangeSE_SM,
        AudParams::arrangeSE_RS,
        AudParams::arrangeHOMESE,
    });
}

void AudSystem::setMeResource(JKRArchive* pArchive, u16 a1, u16 a2) {
    if (pArchive == nullptr) {
        return;
    }

    void* meTable = pArchive->getResource(a1);
    void* meSeq = pArchive->getResource(a2);
    mRhythmMeSystem->mMeMgr->setMeTable(meTable);
    mRhythmMeSystem->mMeMgr->setMeSeq(meSeq);
}

void AudSystem::seFanfareProcess() {
    if (mBgmMgr.mBgm[AudBgmMgr::BgmType_Stage] != nullptr) {
        switch (mBgmMgr.mBgm[AudBgmMgr::BgmType_Stage]->getSoundID()) {
        case STM_BOSS_01_A:
        case STM_BOSS_02_A:
        case STM_BOSS_02_B:
        case STM_BOSS_03_A:
        case STM_BOSS_03_B:
        case STM_RACE_01:
        case STM_BOSS_04:
        case STM_BOSS_05_A:
        case STM_BOSS_06_A:
        case STM_BOSS_06_B:
        case STM_BOSS_01_B:
        case STM_BOSS_05_B:
        case STM_KINOPIO_TANKEN:
        case STM_BOSS_09_A:
        case STM_BOSS_09_B:
            return;
        }
    }

    if (mBgmMgr.mBgm[AudBgmMgr::BgmType_Sub] != nullptr) {
        switch (mBgmMgr.mBgm[AudBgmMgr::BgmType_Sub]->getSoundID()) {
        case BGM_MISS:
        case BGM_GAMEOVER:
        case BGM_PINCH_1:
            mBgmMgr.volDownLevel(true);
            return;
        }
    }

    bool firstMorph = false;
    if (getNumOfPlaying(SE_SY_MORPH) != 0) {
        if (mBgmMgr.mBgm[AudBgmMgr::BgmType_Sub] != nullptr) {
            if (mBgmMgr.mBgm[AudBgmMgr::BgmType_Sub]->getSoundID() == BGM_FIRST_MORPH_B) {
                firstMorph = true;
            }
        }
    }

    if (isFanfareSePlaying()) {
        mBgmMgr.volDownLevel(firstMorph);
    }
}

bool AudSystem::isFanfareSePlaying() {
    AudSoundInfo* info = AudSoundInfo::getInstance();
    for (s32 category = 0; category < JAISeMgr::NUM_CATEGORIES; category++) {
        for (JSULink< JAISe >* it = getSeMgr().getCategory(category)->getSeList()->getFirst(); it != getSeMgr().getCategory(0)->getSeList()->getEnd();
             it = it->getNext()) {
            if (info->isFanfareSe(it->getObject()->mSoundID)) {
                return true;
            }
        }
    }
    return false;
}

void AudSystem::registerLimitedSound(JAISoundID soundID, s32 delay) {
    stopSoundID(soundID);

    for (s32 i = 0; i < 2; i++) {
        if (mLimitedSoundInfo[i].mSoundID == soundID) {
            return;
        }
    }

    for (s32 i = 0; i < 2; i++) {
        if (mLimitedSoundInfo[i].isFree()) {
            mLimitedSoundInfo[i].set(soundID, delay);
            break;
        }
    }
}

void AudSystem::clearAllLimitedSound() {
    for (s32 i = 0; i < 2; i++) {
        mLimitedSoundInfo[i].init();
    }
}

bool AudSystem::isRegisteredLimitedSound(JAISoundID soundID) {
    for (s32 i = 0; i < 2; i++) {
        if (mLimitedSoundInfo[i].mSoundID == soundID) {
            return true;
        }
    }
    return false;
}

void AudSystem::updateLimitedSound() {
    for (s32 i = 0; i < 2; i++) {
        mLimitedSoundInfo[i].update();
    }
}

bool AudSystem::isAlreadyPlayingSoundNear(JAISoundID soundID, const TVec3f* pPos, f32 range) {
    TVec3f pos;
    for (s32 category = 0; category < JAISeMgr::NUM_CATEGORIES; category++) {
        for (JSULink< JAISe >* it = getSeMgr().getCategory(category)->getSeList()->getFirst(); it != getSeMgr().getCategory(0)->getSeList()->getEnd();
             it = it->getNext()) {
            if (it->getObject()->mSoundID != soundID) {
                continue;
            }

            bool valid;
            JAIAudible* audible = it->getObject()->mAudible;
            if (audible != nullptr) {
                pos.set(audible->getPos());
                valid = true;
            } else {
                valid = false;
            }

            if (!valid) {
                continue;
            }

            TVec3f diff(0.0f);
            diff.sub(pos, *pPos);
            if (diff.length() <= range && diff.length() != 0.0f) {
                return true;
            }
        }
    }

    return false;
}

void AudSystem::setVolumeZeroForce(s32 menuType) {
    for (s32 category = 0; category < JAISeMgr::NUM_CATEGORIES; category++) {
        if (category == 0xD) {  // HOMESE
            continue;
        }

        if (menuType == MenuType_Pause && category == 0x0) {  // pause and SE_SY
            continue;
        }

        mSeMgr.getCategory(category)->stop(18);
    }
}

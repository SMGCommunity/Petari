#include "Game/Util/SoundUtil.hpp"
#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/AudioLib/AudMeNameConverter.hpp"
#include "Game/AudioLib/AudMicWrap.hpp"
#include "Game/AudioLib/AudRemixMgr.hpp"
#include "Game/AudioLib/AudSceneMgr.hpp"
#include "Game/AudioLib/AudSoundNameConverter.hpp"
#include "Game/AudioLib/AudSpeakerWrap.hpp"
#include "Game/AudioLib/AudSystem.hpp"
#include "Game/AudioLib/AudWrap.hpp"
#include "Game/AudioLib/CSSoundNameConverter.hpp"
#include "Game/GameAudio/AudBgmConductor.hpp"
#include "Game/GameAudio/AudEffectDirector.hpp"
#include "Game/GameAudio/AudSeKeeper.hpp"
#include "Game/GameAudio/AudStageBgmWrap.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/RhythmLib/AudChordInfo.hpp"
#include "Game/RhythmLib/AudMeObject.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/System/ResourceHolder.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include <JSystem/JAudio2/JAISound.hpp>

namespace {
    AudBgmConductor* getAudBgmConductor() {
        return MR::getSceneObj< AudBgmConductor >(SceneObj_AudBgmConductor);
    }
};  // namespace

namespace MR {
    JAISoundHandle* startSystemSE(JAISoundID id, s32 param2, s32 param3) {
        return AudWrap::getSystemSeObject()->startSoundParam(id, param2, param3);
    }

    JAISoundHandle* startSystemLevelSE(const char* pName, s32 param2, s32 param3) {
        return AudWrap::getSystemSeObject()->startLevelSoundParam(pName, param2, param3);
    }

    JAISoundHandle* startSystemLevelSE(JAISoundID id, s32 param2, s32 param3) {
        return AudWrap::getSystemSeObject()->startLevelSoundParam(id, param2, param3);
    }

    void stopSystemSE(const char* pName, u32 param2) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        stopSystemSE(id, param2);
    }

    void stopSystemSE(JAISoundID id, u32 param2) {
        AudWrap::getSystemSeObject()->stopSound(id, param2);
    }

    JAISoundHandle* startAtmosphereSE(const char* pName, s32 param2, s32 param3) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return AudWrap::getAtmosphereSeObject()->startSoundParam(id, param2, param3);
    }

    JAISoundHandle* startAtmosphereLevelSE(const char* pName, s32 param2, s32 param3) {
        return AudWrap::getAtmosphereSeObject()->startLevelSoundParam(pName, param2, param3);
    }

    JAISoundHandle* startSoundObject(AudSoundObject* pSoundObject, const char* pName) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return pSoundObject->startSound(id);
    }

    JAISoundHandle* startSoundObjectLevel(AudSoundObject* pSoundObject, const char* pName, s32 param3) {
        JAISoundHandle* pSoundHandle = pSoundObject->startLevelSound(pName);

        if (param3 > 0 && pSoundHandle != nullptr) {
            (*pSoundHandle)->updateLifeTime(param3);
        }

        return pSoundHandle;
    }

    JAISoundHandle* startSoundObjectLevel(AudSoundObject* pSoundObject, JAISoundID id, s32 param3) {
        JAISoundHandle* pSoundHandle = pSoundObject->startLevelSound(id);

        if (param3 > 0 && pSoundHandle != nullptr) {
            (*pSoundHandle)->updateLifeTime(param3);
        }

        return pSoundHandle;
    }

    JAISoundHandle* startSoundObjectLevelParam(AudSoundObject* pSoundObject, const char* pName, s32 param3, s32 param4, s32 param5) {
        JAISoundHandle* pSoundHandle = pSoundObject->startLevelSoundParam(pName, param3, param4);

        if (param5 > 0 && pSoundHandle != nullptr) {
            (*pSoundHandle)->updateLifeTime(param5);
        }

        return pSoundHandle;
    }

    JAISoundHandle* startSoundObjectLevelParam(AudSoundObject* pSoundObject, JAISoundID id, s32 param3, s32 param4, s32 param5) {
        JAISoundHandle* pSoundHandle = pSoundObject->startLevelSoundParam(id, param3, param4);

        if (param5 > 0 && pSoundHandle != nullptr) {
            (*pSoundHandle)->updateLifeTime(param5);
        }

        return pSoundHandle;
    }

    JAISoundHandle* startSound(const LiveActor* pActor, const char* pName, s32 param3, s32 param4) {
        pActor->mSoundObject->setMapCode(getMapSoundCodeFoot(pActor));

        if (param3 != -1) {
            JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

            return pActor->mSoundObject->startSoundParam(id, param3, param4);
        } else {
            JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

            return pActor->mSoundObject->startSound(id);
        }
    }

    JAISoundHandle* startSound(const LiveActor* pActor, JAISoundID id, s32 param3, s32 param4) {
        pActor->mSoundObject->setMapCode(getMapSoundCodeFoot(pActor));

        if (param3 != -1) {
            return pActor->mSoundObject->startSoundParam(id, param3, param4);
        } else {
            return pActor->mSoundObject->startSound(id);
        }
    }

    JAISoundHandle* startSoundSeVer(const LiveActor* pActor, const char* pName, s32 param3, s32 param4) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);
        JAISoundID id2 = pActor->mSoundObject->convertSoundIdFromSeVersion(id);

        return startSound(pActor, id2, param3, param4);
    }

    JAISoundHandle* startLevelSound(const LiveActor* pActor, const char* pName, s32 param3, s32 param4, s32 param5) {
        pActor->mSoundObject->setMapCode(getMapSoundCodeFoot(pActor));

        if (param3 != -1) {
            return startSoundObjectLevelParam(pActor->mSoundObject, pName, param3, param4, param5);
        } else {
            return startSoundObjectLevel(pActor->mSoundObject, pName, param5);
        }
    }

    JAISoundHandle* startLevelSound(const LiveActor* pActor, JAISoundID id, s32 param3, s32 param4, s32 param5) {
        pActor->mSoundObject->setMapCode(getMapSoundCodeFoot(pActor));

        if (param3 != -1) {
            return startSoundObjectLevelParam(pActor->mSoundObject, id, param3, param4, param5);
        } else {
            return startSoundObjectLevel(pActor->mSoundObject, id, param5);
        }
    }

    JAISoundHandle* startLevelSoundSeVer(const LiveActor* pActor, const char* pName, s32 param3, s32 param4, s32 param5) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);
        JAISoundID id2 = pActor->mSoundObject->convertSoundIdFromSeVersion(id);

        return startLevelSound(pActor, id2, param3, param4, param5);
    }

    void stopSound(const LiveActor* pActor, const char* pName, u32 param3) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        stopSound(pActor, id, param3);
    }

    void stopSound(const LiveActor* pActor, JAISoundID id, u32 param3) {
        pActor->mSoundObject->stopSound(id, param3);
    }

    void setSeVersion(const LiveActor* pActor, u32 version) {
        pActor->mSoundObject->_30 = version;
    }

    void releaseSoundHandle(const LiveActor* pActor, const char* pName) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        pActor->mSoundObject->releaseHandle(id);
    }

    void startBas(const LiveActor* pActor, const char* pName, bool param3, f32 param4, f32 param5) {
        if (pActor->mSoundObject == nullptr) {
            return;
        }

        ResourceHolder* pResourceHolder = getResourceHolder(pActor);
        JAUSoundAnimation* pSoundAnimation = nullptr;

        if (pResourceHolder->mBasResTable->isExistRes(pName)) {
            pSoundAnimation = static_cast< JAUSoundAnimation* >(pResourceHolder->mBasResTable->getRes(pName));
        }

        pActor->mSoundObject->setMapCode(getMapSoundCodeFoot(pActor));
        pActor->mSoundObject->startAnimation(pSoundAnimation, param3, param4, param5);
    }

    void actorSoundMovement(LiveActor* pActor) {
        AudAnmSoundObject* pSoundObject = pActor->mSoundObject;

        if (pSoundObject == nullptr) {
            return;
        }

        pSoundObject->setMapCode(getMapSoundCodeFoot(pActor));

        if (pActor->mModelManager != nullptr) {
            ResourceHolder* pResourceHolder = getResourceHolder(pActor);

            if (pResourceHolder->mMotionResTable->mCount != 0) {
                f32 frame = getBckCtrl(pActor)->mFrame;

                pSoundObject->update(frame);
            }
        }

        pSoundObject->process();
    }

    void addJointToSeKeeper(AudSeKeeper* pSeKeeper, const char* pParam2, const char* pParam3) {
        pSeKeeper->makeBox(pParam2, pParam3);
    }

    void startLevelSoundSeKeeper(AudSeKeeper* pSeKeeper, const char* pParam2, const char* pParam3) {
        pSeKeeper->playLevel(pParam2, pParam3);
    }

    bool hasME() {
        AudChordInfo* pChordInfo = AudWrap::getSystem()->getChordInfo();

        return pChordInfo->mTable.mLoaded && pChordInfo->mCurChord != nullptr && pChordInfo->mCurScale != nullptr;
    }

    void startSystemME(const char* pName) {
        if (!hasME()) {
            return;
        }

        u32 id = AudSingletonHolder< AudMeNameConverter >::get()->getMeID(pName);

        AudWrap::getSystemMeObject()->startMe(id);
    }

    // getMapSoundCodeFoot

    void setMapSondCodeGravity(const LiveActor* pActor, s32 code) {
        if (pActor->mSoundObject == nullptr) {
            return;
        }

        pActor->mSoundObject->setMapCodeExtra(code);
    }

    // startTalkSound

    void startRemixSound(s32 melodyNo, s32 param2, f32 param3) {
        AudWrap::getRemixMgr()->getRemixNoteGroupDataFromMelodyNo(melodyNo);

        AudRemixSequencer* pRemixSequencer = AudWrap::getRemixSequencer();
        f32 playerSpeed = getPlayerVelocity()->length();

        if (playerSpeed < 3.0f) {
            playerSpeed = 3.0f;
        }

        pRemixSequencer->setTempo((playerSpeed * 3600.0f) / param3);

        RemixNoteGroupData* pRemixNoteGroupData = AudWrap::getRemixMgr()->getRemixNoteGroupDataFromMelodyNo(melodyNo);

        for (s32 i = 0; i < pRemixNoteGroupData->mTrackCount; i++) {
            RemixNoteTrackData* pRemixNoteTrackData = &pRemixNoteGroupData->mRemixTracks[i];

            pRemixSequencer->addNoteData(pRemixNoteTrackData, &pRemixNoteTrackData->_4[param2]);
        }
    }

    s32 getRemixMelodyNoteNum(s32 melodyNo) {
        return AudWrap::getRemixMgr()->getRemixNoteGroupDataFromMelodyNo(melodyNo)->mNoteCount;
    }

    void limitedSound(const char* pName, s32 param2) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        AudWrap::getSystem()->registerLimitedSound(id, param2);
    }

    JAISoundHandle* startStageBGM(const char* pName, bool param2) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return AudWrap::startStageBgm(id, param2);
    }

    JAISoundHandle* startStageBGMFromStageName(const char* pSceneName, const char* pStageName, s32 scenarioNo) {
        JAISoundID id = AudStageBgmWrap::changeStageNameToSoundID(pSceneName, pStageName, scenarioNo);

        if (id.isAnonymous()) {
            return nullptr;
        }

        JAISoundHandle* pSoundHandle = AudWrap::startStageBgm(id, false);

        if (pSoundHandle != nullptr) {
            AudSceneMgr* pSceneMgr = AudWrap::getSceneMgr();

            pSceneMgr->_4 = 1;
        }

        return pSoundHandle;
    }

    void stopStageBGM(u32 param1) {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            pStageBgm->stop(param1);
        }
    }

    void unlockStageBGM() {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            pStageBgm->playAfterPrepared();
        }
    }

    void setNextStageBGM(const char* pName) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        AudWrap::setNextIdStageBgm(id);
    }

    void moveVolumeStageBGM(f32 param1, u32 param2) {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            pStageBgm->moveVolume(param1, param2);
        }
    }

    void moveVolumeStageBGMForNoteFairy(f32 param1, u32 param2) {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            pStageBgm->moveVolumeForNoteFairy(param1, param2);
        }
    }

    JAISoundHandle* startLastStageBGM() {
        return AudWrap::startLastStageBgm();
    }

    bool isPlayingStageBgm() {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            return !pStageBgm->isStopping();
        }

        return false;
    }

    bool isPlayingStageBgmID(u32 id) {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr && !pStageBgm->isStopping() && pStageBgm->getSoundID() == id) {
            return true;
        }

        return false;
    }

    bool isPlayingStageBgmName(const char* pName) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return isPlayingStageBgmID(id);
    }

    // isStopOrFadeoutStageBgmID

    bool isStopOrFadeoutBgmName(const char* pName) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return isStopOrFadeoutStageBgmID(id);
    }

    bool isPreparedStageBgm() {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            return pStageBgm->isPreparedPlay();
        }

        return false;
    }

    void setStageBGMState(s32 state, u32 time) {
        AudBgm* pStageBgm = AudWrap::getStageBgm();

        if (pStageBgm != nullptr) {
            pStageBgm->changeTrackMuteState(state, time);
        }
    }

    void setStageBGMStateBit(u32 param1) {
        AudBgmConductor* pBgmConductor = ::getAudBgmConductor();

        if (pBgmConductor != nullptr) {
            pBgmConductor->setStateBit(param1);
        }
    }

    JAISoundHandle* startSubBGM(const char* pName, bool param2) {
        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return AudWrap::startSubBgm(id, param2);
    }

    void stopSubBGM(u32 param1) {
        AudBgm* pSubBgm = AudWrap::getSubBgm();

        if (pSubBgm != nullptr) {
            pSubBgm->stop(param1);
        }
    }

    void moveVolumeSubBGM(f32 param1, u32 param2) {
        AudBgm* pSubBgm = AudWrap::getSubBgm();

        if (pSubBgm != nullptr) {
            pSubBgm->moveVolume(param1, param2);
        }
    }

    bool isPlayingSubBgmID(u32 id) {
        AudBgm* pSubBgm = AudWrap::getSubBgm();

        if (pSubBgm != nullptr && !pSubBgm->isStopping() && pSubBgm->getSoundID() == id) {
            return true;
        }

        return false;
    }

    void playLevelMarioPinchBGM(bool param1) {
        if (isPlayerDead()) {
            return;
        }

        AudBgmConductor* pBgmConductor = ::getAudBgmConductor();

        if (pBgmConductor != nullptr) {
            pBgmConductor->playPinchEventBgmLevel(param1);
        }
    }

    void stopMarioPinchBGMSoon() {
        AudBgmConductor* pBgmConductor = ::getAudBgmConductor();

        if (pBgmConductor != nullptr) {
            pBgmConductor->stopPinchEventBgmSoon();
        }
    }

    void submitTrigSE() {
        AudSystem* pSystem = AudWrap::getSystem();

        pSystem->_82B = 1;
    }

    void permitTrigSE() {
        AudSystem* pSystem = AudWrap::getSystem();

        pSystem->_82B = 0;
    }

    void submitLevelSE() {
        AudSystem* pSystem = AudWrap::getSystem();

        pSystem->_82C = 1;
    }

    void permitLevelSE() {
        AudSystem* pSystem = AudWrap::getSystem();

        pSystem->_82C = 0;
    }

    void submitSE() {
        submitTrigSE();
        submitLevelSE();
    }

    void permitSE() {
        permitTrigSE();
        permitLevelSE();
    }

    bool isPermitSE() {
        return !AudWrap::getSystem()->_82B && !AudWrap::getSystem()->_82C;
    }

    void setCubeBgmChangeInvalid() {
        AudSceneMgr* pSceneMgr = AudWrap::getSceneMgr();

        pSceneMgr->_1D = 1;
    }

    bool isCubeBgmChangeInvalid() {
        return AudWrap::getSceneMgr()->_1D;
    }

    void clearBgmQueue() {
        AudWrap::getBgmMgr()->clearLastBGM(0);
        AudWrap::getBgmMgr()->clearLastBGM(1);
        AudWrap::getBgmMgr()->clearNextBGM(0);
        AudWrap::getBgmMgr()->clearNextBGM(1);
    }

    void setMicMtx(MtxPtr pMtx, s32 param2) {
        AudMicWrap::setMicMtx(pMtx, param2);
    }

    void setSoundVolumeSetting(s32 param1, u32 param2) {
        AudWrap::getSystem()->setSeVolumeSet(param1, param2);
    }

    void recoverSoundVolumeSetting(u32 param1) {
        AudWrap::getSystem()->recoverSeVolumeSet(param1);
    }

    void setAudioEffectType(s32 param1, s32 param2) {
        MR::getSceneObj< AudEffectDirector >(SceneObj_AudEffectDirector)->setEffectType(param1, param2);
    }

    void startCSSound(const char* pCSName, const char* pSEName, s32 param3) {
        if (AudSpeakerWrap::isPlayable(-1)) {
            s32 id = AudSingletonHolder< CSSoundNameConverter >::get()->getSoundID(pCSName);

            if (id != -1) {
                AudSpeakerWrap::start(id, param3);
            }
        } else if (pSEName != nullptr) {
            startSystemSE(pSEName);
        }
    }

    void startCSSound2P(const char* pCSName, const char* pSEName) {
        if (isConnectedWPad(1)) {
            startCSSound(pCSName, pSEName, 1);
        }
    }

    JAISoundHandle* startSystemSE(const char* pName, s32 param2, s32 param3) {
        if (MR::isEqualString(pName, "SE_SY_READ_RIDDLE_S") && MR::isPlayerDead()) {
            return nullptr;
        }

        JAISoundID id = AudSingletonHolder< AudSoundNameConverter >::get()->getSoundID(pName);

        return startSystemSE(id, param2, param3);
    }

    void startSpinHitSound(const LiveActor* pActor) {
        startCSSound("CS_SPIN_HIT", nullptr, 0);
    }

    void startBlowHitSound(const LiveActor* pActor) {
        startSound(pActor, "SE_PM_SPIN_HIT");
    }

    void startDPDHitSound() {
        if (hasME()) {
            startSystemME("ME_DPD_HIT");
        } else {
            startSystemSE("SE_SY_DPD_HIT");
        }

        startCSSound2P("CS_DPD_HIT", nullptr);
    }

    void startDPDFreezeLevelSound(const LiveActor* pActor) {
        startLevelSound(pActor, "SE_EM_LV_DPD_POINT");
    }

    void start2PJumpAssistSound() {
        if (hasME()) {
            startSystemME("ME_2P_ASSIST_JUMP");
        } else {
            startSystemSE("SE_SY_2P_ASSIST_JUMP");
        }

        startCSSound2P("CS_DPD_JUMP", nullptr);
    }

    void start2PJumpAssistJustSound() {
        if (hasME()) {
            startSystemME("ME_2P_ASSIST_JUMP_L");
        } else {
            startSystemSE("SE_SY_2P_ASSIST_JUMP_L");
        }

        startCSSound2P("CS_DPD_JUMP_HIGH", nullptr);
    }

    void start2PAttackAssistSound() {
        if (hasME()) {
            startSystemME("ME_2P_ASSIST_ATTACK");
        } else {
            startSystemSE("SE_SY_2P_ASSIST_ATTACK");
        }

        startCSSound2P("CS_DPD_PUNCH", nullptr);
    }

    void limitedStarPieceHitSound() {
        limitedSound("SE_OJ_STAR_PIECE_HIT_ENEMY", 1);
    }

    void startCurrentStageBGM() {
        startStageBGMFromStageName("Game", getCurrentStageName(), getCurrentScenarioNo());
    }

    JAISoundHandle* startBossBGM(s32 id) {
        JAISoundHandle* pSoundHandle = nullptr;

        switch (id) {
        case BossBgmID_01_A:
            pSoundHandle = startStageBGM("MBGM_BOSS_01_A", 0);
            break;
        case BossBgmID_01_B:
            pSoundHandle = startStageBGM("MBGM_BOSS_01_B", 0);
            break;
        case BossBgmID_02_A:
            pSoundHandle = startStageBGM("MBGM_BOSS_02_A", 0);
            break;
        case BossBgmID_02_B:
            pSoundHandle = startStageBGM("MBGM_BOSS_02_B", 0);
            break;
        case BossBgmID_03_A:
            pSoundHandle = startStageBGM("MBGM_BOSS_03_A", 0);
            break;
        case BossBgmID_03_B:
            pSoundHandle = startStageBGM("MBGM_BOSS_03_B", 0);
            break;
        case BossBgmID_05_A:
            pSoundHandle = startStageBGM("MBGM_BOSS_05_A", 0);
            break;
        case BossBgmID_05_B:
            pSoundHandle = startStageBGM("MBGM_BOSS_05_B", 0);
            break;
        }

        return pSoundHandle;
    }

    JAISoundHandle* startEventBGM(s32 id) {
        JAISoundHandle* pSoundHandle = nullptr;

        switch (id) {
        case EventBgmID_Hurry:
            pSoundHandle = startStageBGM("MBGM_GALAXY_02_HURRY", 0);
            break;
        case EventBgmID_Chase:
            pSoundHandle = startStageBGM("MBGM_GALAXY_02_CHASE", 0);
            break;
        case EventBgmID_Race_01:
            pSoundHandle = startStageBGM("MBGM_RACE_01", 0);
            break;
        case EventBgmID_Race_02:
            pSoundHandle = startStageBGM("MBGM_RACE_02", 0);
            break;
        }

        return pSoundHandle;
    }

    JAISoundHandle* startAfterBossBGM() {
        setCubeBgmChangeInvalid();
        clearBgmQueue();

        if (isPlayerDead()) {
            return nullptr;
        }

        if (!isGalaxyDarkCometAppearInCurrentStage()) {
            JAISoundID id = AudStageBgmWrap::getCometEventBgm(getCurrentStageName());

            if (!id.isAnonymous()) {
                return AudWrap::startStageBgm(id, false);
            }
        }

        return startStageBGM("MBGM_STAR_EXIST", 0);
    }
};  // namespace MR

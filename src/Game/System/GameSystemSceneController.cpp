#include "Game/System/GameSystemSceneController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Scene/ScenarioSelectScene.hpp"
#include "Game/Scene/SceneFactory.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/System/AudSystemWrapper.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemObjHolder.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <cstdio>

namespace {
    AudSystemWrapper* getAudioSystemWrapper() NO_INLINE {
        return SingletonHolder< GameSystem >::get()->mObjHolder->mAudioSystem;
    }
};  // namespace

namespace NrvGameSystemSceneController {
    NEW_NERVE(GameSystemSceneControllerNotInitialized, GameSystemSceneController, NotInitialized);
    NEW_NERVE(GameSystemSceneControllerNormal, GameSystemSceneController, Normal);
    NEW_NERVE(GameSystemSceneControllerChangeWaveBank, GameSystemSceneController, ChangeWaveBank);
    NEW_NERVE(GameSystemSceneControllerInitializeScene, GameSystemSceneController, InitializeScene);
    NEW_NERVE(GameSystemSceneControllerInvalidateSystemWipe, GameSystemSceneController, InvalidateSystemWipe);
    NEW_NERVE(GameSystemSceneControllerWaitDrawDoneScene, GameSystemSceneController, WaitDrawDoneScene);
    NEW_NERVE(GameSystemSceneControllerDestroyScene, GameSystemSceneController, DestroyScene);
    NEW_NERVE(GameSystemSceneControllerReadyToStartScene, GameSystemSceneController, ReadyToStartScene);
    NEW_NERVE(GameSystemSceneControllerWaitDrawDoneSceneForDestroy, GameSystemSceneController, WaitDrawDoneScene);
    NEW_NERVE(GameSystemSceneControllerDestroySceneForDestroy, GameSystemSceneController, DestroyScene);
    NEW_NERVE(GameSystemSceneControllerDestroyed, GameSystemSceneController, Destroyed);
};  // namespace NrvGameSystemSceneController

SceneControlInfo::SceneControlInfo() : mStartIdInfo() {
    mScene[0] = '\0';
    mStage[0] = '\0';
    mScenarioNo = 1;
    mSelectedScenarioNo = 1;
    mStartIdInfo = new JMapIdInfo(MR::getInitializeStartIdInfo());
}

void SceneControlInfo::setScene(const char* pSceneName) {
    snprintf(mScene, sizeof(mScene), "%s", pSceneName);
}

void SceneControlInfo::setStage(const char* pStageName) {
    snprintf(mStage, sizeof(mStage), "%s", pStageName);
}

void SceneControlInfo::setStartIdInfo(const JMapIdInfo& rInfo) {
    *mStartIdInfo = rInfo;
}

GameSystemSceneController::GameSystemSceneController()
    : mSpine(), mNextNerve(), mIsResetProcessing(), mScenarioParser(), mObjHolder(), mScene(), mSceneInitializeState(), mIntermissionScene(),
      mPlayTimerScene(), mScenarioSelectScene() {
    mObjHolder = new NameObjHolder(0x1300);
    mSpine = new Spine(this, &NrvGameSystemSceneController::GameSystemSceneControllerNotInitialized::sInstance);
    mIntermissionScene = static_cast< IntermissionScene* >(MR::createScene("Intermission"));
    mPlayTimerScene = new PlayTimerScene();
    mScenarioSelectScene = new ScenarioSelectScene();
}

void GameSystemSceneController::initAfterStationedResourceLoaded() {
    mScenarioParser = new ScenarioDataParser("シナリオデータ解析");
    mScenarioParser->initWithoutIter();

    mPlayTimerScene->init();
    mScenarioSelectScene->init();
}

void GameSystemSceneController::requestChangeScene() {
    if (isExistRequest()) {
        return;
    }

    if (mIsResetProcessing) {
        return;
    }

    mSceneInitializeState = SceneInitializeState_NotInit;

    requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance);
}

void GameSystemSceneController::checkRequestAndChangeScene() {
    if (mNextNerve != nullptr) {
        mSpine->setNerve(mNextNerve);
        mNextNerve = nullptr;
    }

    mSpine->update();
}

void GameSystemSceneController::initializeScene() {
    mSceneInitializeState = SceneInitializeState_Init;

    if (SingletonHolder< HeapMemoryWatcher >::get()->mFileCacheHeap == nullptr || !isSameAtNextSceneAndStage()) {
        SingletonHolder< HeapMemoryWatcher >::get()->createFileCacheHeapOnGameHeap(0x1040400);
    }

    SingletonHolder< HeapMemoryWatcher >::get()->createSceneHeapOnGameHeap();
    SingletonHolder< HeapMemoryWatcher >::get()->setCurrentHeapToSceneHeap();
    updateSceneControlInfo();
    MR::setRandomSeedFromStageName();

    if (mScenarioSelectScene->isActive()) {
        mScenarioSelectScene->validateScenarioSelect();
    }

    MR::resetWPad();

    mScene = MR::createScene(mCurrSceneControlInfo.mScene);
    mScene->initNameObjListExecutor();
    mScene->initSceneObjHolder();
    mScene->init();

    SceneFunction::allocateDrawBufferActorList();
    MR::clearFileLoaderRequestFileInfo(mIsResetProcessing);
    SingletonHolder< HeapMemoryWatcher >::get()->checkRestMemory();
}

void GameSystemSceneController::destroyScene() {
    bool stopSound = isStopSound();

    Scene* pScene = mScene;
    mScene = nullptr;
    delete pScene;

    mObjHolder->clearArray();

    if (stopSound) {
        MR::stopAllSound(0);
    }

    GameSystemFunction::resetAllControllerRumble();
    MR::removeResourceAndFileHolderIfIsEqualHeap(MR::getSceneHeapNapa());

    if (tryDestroyFileCacheHeap(false)) {
        SingletonHolder< HeapMemoryWatcher >::get()->destroyGameHeap();
    } else {
        SingletonHolder< HeapMemoryWatcher >::get()->destroySceneHeap();
    }
}

bool GameSystemSceneController::isStopSound() const {
    if (MR::isEqualSceneName("Title")) {
        return false;
    }

    if (MR::isEqualStageName("AstroDome") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game")) {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game") &&
        MR::isEqualString(mNextSceneControlInfo.mStage, "AstroDome")) {
        return false;
    }

    if (MR::isEqualStageName("LibraryRoom") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game")) {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game") &&
        MR::isEqualString(mNextSceneControlInfo.mStage, "LibraryRoom")) {
        return false;
    }

    if (MR::isEqualStageName("HeavensDoorGalaxy") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game")) {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy") && MR::isEqualString(mNextSceneControlInfo.mScene, "Game") &&
        MR::isEqualString(mNextSceneControlInfo.mStage, "HeavensDoorGalaxy")) {
        return false;
    }

    return true;
}

bool GameSystemSceneController::isReadyToStartScene() const {
    return isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerReadyToStartScene::sInstance) &&
           !mScenarioSelectScene->isScenarioSelecting();
}

bool GameSystemSceneController::isFirstUpdateSceneNerveNormal() const {
    return isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance) && mSpine->mStep == 1;
}

void GameSystemSceneController::startScene() {
    mScene->start();
    GameSystemFunction::restartControllerLeaveWatcher();
    requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance);
}

void GameSystemSceneController::updateScene() {
    if (mIsResetProcessing) {
        return;
    }

    Scene* pScene = getCurrentSceneForExecute();

    if (pScene != nullptr) {
        pScene->update();
        mPlayTimerScene->update();
        mScenarioSelectScene->update();
    }
}

void GameSystemSceneController::updateSceneDuringResetProcessing() {
    if (mIsResetProcessing) {
        mScenarioSelectScene->update();
    }
}

void GameSystemSceneController::calcAnimScene() {
    Scene* pScene = getCurrentSceneForExecute();

    if (pScene != nullptr) {
        pScene->calcAnim();
        mPlayTimerScene->calcAnim();
        mScenarioSelectScene->calcAnim();
    }
}

void GameSystemSceneController::drawScene() {
    Scene* pScene = getCurrentSceneForExecute();

    if (pScene != nullptr) {
        pScene->draw();
        mPlayTimerScene->draw();
        mScenarioSelectScene->draw();
    }
}

NameObjListExecutor* GameSystemSceneController::getNameObjListExecutor() const {
    return mScene->mListExecutor;
}

SceneObjHolder* GameSystemSceneController::getSceneObjHolder() const {
    return mScene->mSceneObjHolder;
}

bool GameSystemSceneController::isExistSceneObjHolder() const {
    return mScene != nullptr && mScene->mSceneObjHolder != nullptr;
}

s32 GameSystemSceneController::getCurrentScenarioNo() const {
    return mCurrSceneControlInfo.mScenarioNo;
}

s32 GameSystemSceneController::getCurrentSelectedScenarioNo() const {
    return mCurrSceneControlInfo.mSelectedScenarioNo;
}

void GameSystemSceneController::setSceneInitializeState(SceneInitializeState state) {
    mSceneInitializeState = state;
}

bool GameSystemSceneController::isSceneInitializeState(SceneInitializeState state) const {
    return mSceneInitializeState == state;
}

void GameSystemSceneController::startScenarioSelectScene() {
    mScenarioSelectScene->start();
}

void GameSystemSceneController::startScenarioSelectSceneBackground() {
    mScenarioSelectScene->startBackground();
}

void GameSystemSceneController::setCurrentScenarioNo(s32 scenarioNo, s32 selectedScenarioNo) {
    mCurrSceneControlInfo.mScenarioNo = scenarioNo;
    mCurrSceneControlInfo.mSelectedScenarioNo = selectedScenarioNo;
}

void GameSystemSceneController::resetCurrentScenarioNo() {
    mCurrSceneControlInfo.mScenarioNo = -1;
}

bool GameSystemSceneController::isScenarioDecided() const {
    return mCurrSceneControlInfo.mScenarioNo > 0;
}

void GameSystemSceneController::loadScenarioWaveData() {
    ::getAudioSystemWrapper()->loadScenarioWaveData(mCurrSceneControlInfo.mScene, mCurrSceneControlInfo.mStage,
                                                    mCurrSceneControlInfo.mScenarioNo);
}

bool GameSystemSceneController::isLoadDoneScenarioWaveData() const {
    return ::getAudioSystemWrapper()->isLoadDoneScenarioWaveData();
}

void GameSystemSceneController::exeNotInitialized() {
    if (mSpine->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[system setup]");
    }
}

void GameSystemSceneController::exeNormal() {
}

void GameSystemSceneController::exeWaitDrawDoneScene() {
    const Nerve* pNerve;

    if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance)) {
        if (mScene != nullptr) {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyScene::sInstance;
        } else {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance;
        }
    } else if (mScene != nullptr) {
        pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance;
    } else {
        pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
    }

    requestChangeNerve(pNerve);
}

void GameSystemSceneController::exeDestroyScene() {
    if (mSpine->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[destroy Scene: %s]", mCurrSceneControlInfo.mScene);
        MR::startFunctionAsyncExecute(MR::Functor(this, &GameSystemSceneController::destroyScene), 17, "シーン破棄");
    }

    if (MR::tryEndFunctionAsyncExecute("シーン破棄")) {
        const Nerve* pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance;

        if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance)) {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
        }

        requestChangeNerve(pNerve);
    }
}

void GameSystemSceneController::exeChangeWaveBank() {
    if (mSpine->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[change Wave Data]");

        bool isPlayerLuigi = GameSystemFunction::isCreatedGameDataHolder() ? MR::isPlayerLuigi() : false;

        ::getAudioSystemWrapper()->loadStageWaveData(mNextSceneControlInfo.mScene, mNextSceneControlInfo.mStage, isPlayerLuigi);
    }

    if (::getAudioSystemWrapper()->isLoadDoneStageWaveData()) {
        requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance);
    }
}

void GameSystemSceneController::exeInitializeScene() {
    if (mSpine->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[initialize Scene: %s]", mNextSceneControlInfo.mScene);
        MR::startFunctionAsyncExecute(MR::Functor(this, &GameSystemSceneController::initializeScene), 17, "シーン初期化");
    }

    if (MR::tryEndFunctionAsyncExecute("シーン初期化")) {
        mSceneInitializeState = SceneInitializeState_End;

        requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInvalidateSystemWipe::sInstance);
    }
}

void GameSystemSceneController::exeInvalidateSystemWipe() {
    bool isValidChangeNerve = false;

    if (!GameSystemFunction::isCreatedSystemWipe()) {
        isValidChangeNerve = true;
    } else if (mScenarioSelectScene->isExecForeground() || mScenarioSelectScene->_16) {
        isValidChangeNerve = true;
    } else if (!MR::isSystemWipeActive()) {
        isValidChangeNerve = true;
    }

    if (isValidChangeNerve) {
        requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerReadyToStartScene::sInstance);
    }
}

void GameSystemSceneController::exeDestroyed() {
    if (mSpine->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[empty scene]");
    }
}

void GameSystemSceneController::exeReadyToStartScene() {
    if (mIsResetProcessing) {
        const Nerve* pNerve = getNextNerveOnResetProcessing();

        if (pNerve != nullptr) {
            requestChangeNerve(pNerve);
        }
    }
}

void GameSystemSceneController::prepareReset() {
    mIsResetProcessing = true;

    mPlayTimerScene->stop();

    bool b = isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance) ||
             isNextNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance);

    mScenarioSelectScene->requestReset(b);

    if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance)) {
        const Nerve* nextNerve = getNextNerveOnResetProcessing();

        if (nextNerve != nullptr) {
            requestChangeNerve(nextNerve);
        }
    }
}

bool GameSystemSceneController::isPreparedReset() {
    return isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance) && mScenarioSelectScene->isResetEnd();
}

void GameSystemSceneController::restartGameAfterResetting() {
    tryDestroyFileCacheHeap(true);
    SingletonHolder< HeapMemoryWatcher >::get()->destroyGameHeap();

    mIsResetProcessing = false;
}

bool GameSystemSceneController::isExistRequest() const {
    return isNextNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance) ||
           isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance);
}

bool GameSystemSceneController::isSameAtNextSceneAndStage() const {
    return MR::isEqualString(mCurrSceneControlInfo.mScene, mNextSceneControlInfo.mScene) &&
           MR::isEqualString(mCurrSceneControlInfo.mStage, mNextSceneControlInfo.mStage) &&
           mCurrSceneControlInfo.mScenarioNo == mNextSceneControlInfo.mScenarioNo;
}

void GameSystemSceneController::updateSceneControlInfo() {
    mCurrSceneControlInfo.setScene(mNextSceneControlInfo.mScene);
    mCurrSceneControlInfo.setStage(mNextSceneControlInfo.mStage);
    mCurrSceneControlInfo.mScenarioNo = mNextSceneControlInfo.mScenarioNo;
    mCurrSceneControlInfo.mSelectedScenarioNo = mNextSceneControlInfo.mSelectedScenarioNo;
    mCurrSceneControlInfo.setStartIdInfo(*mNextSceneControlInfo.mStartIdInfo);

    mNextSceneControlInfo.mScene[0] = '\0';
    mNextSceneControlInfo.mStage[0] = '\0';
    mNextSceneControlInfo.mScenarioNo = -1;
    mNextSceneControlInfo.mSelectedScenarioNo = -1;
}

Scene* GameSystemSceneController::getCurrentSceneForExecute() const {
    if (mScene != nullptr) {
        if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance)) {
            return mScene;
        }
    }

    return mIntermissionScene;
}

const Nerve* GameSystemSceneController::getNextNerveOnResetProcessing() const {
    const Nerve* nextNerve = nullptr;

    if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneSceneForDestroy::sInstance) ||
        isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance) ||
        isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance)) {
    } else if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerDestroyScene::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNotInitialized::sInstance)) {
        nextNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
    } else if (isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInvalidateSystemWipe::sInstance) ||
               isCurrentNerve(&NrvGameSystemSceneController::GameSystemSceneControllerReadyToStartScene::sInstance)) {
        nextNerve = &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneSceneForDestroy::sInstance;
    }

    return nextNerve;
}

bool GameSystemSceneController::tryDestroyFileCacheHeap(bool param1) {
    if (!param1 && SingletonHolder< HeapMemoryWatcher >::get()->mFileCacheHeap != nullptr && isSameAtNextSceneAndStage()) {
        return false;
    }

    MR::removeResourceAndFileHolderIfIsEqualHeap(SingletonHolder< HeapMemoryWatcher >::get()->mFileCacheHeap);

    return true;
}

void GameSystemSceneController::requestChangeNerve(const Nerve* pNerve) {
    const Nerve* pNextNerve;

    if (mIsResetProcessing) {
        pNextNerve = getNextNerveOnResetProcessing();
    } else {
        pNextNerve = nullptr;
    }

    if (pNextNerve == nullptr) {
        pNextNerve = pNerve;
    }

    mNextNerve = pNextNerve;
}

bool GameSystemSceneController::isCurrentNerve(const Nerve* pNerve) const {
    return pNerve == mSpine->getCurrentNerve();
}

bool GameSystemSceneController::isNextNerve(const Nerve* pNerve) const {
    return mNextNerve == pNerve;
}

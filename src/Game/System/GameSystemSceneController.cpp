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
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/SingletonHolder.hpp"
#include <cstdio>

namespace {
    AudSystemWrapper* getAudioSystemWrapper() NO_INLINE {
        return SingletonHolder<GameSystem>::get()->mObjHolder->mSysWrapper;
    }
};

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
};

SceneControlInfo::SceneControlInfo() :
    mStartIdInfo(nullptr)
{
    mScene[0] = '\0';
    mStage[0] = '\0';
    mCurrentScenarioNo = 1;
    mCurrentSelectedScenarioNo = 1;
    mStartIdInfo = new JMapIdInfo(MR::getInitializeStartIdInfo());
}

void SceneControlInfo::setScene(const char *pSceneName) {
    snprintf(mScene, sizeof(mScene), "%s", pSceneName);
}

void SceneControlInfo::setStage(const char *pStageName) {
    snprintf(mStage, sizeof(mStage), "%s", pStageName);
}

void SceneControlInfo::setStartIdInfo(const JMapIdInfo &rInfo) {
    *mStartIdInfo = rInfo;
}

GameSystemSceneController::GameSystemSceneController() :
    _98(nullptr),
    _9C(nullptr),
    _A0(false),
    mScenarioParser(nullptr),
    mObjHolder(nullptr),
    mScene(nullptr),
    mSceneInitializeState(State_NotInit),
    mIntermissionScene(nullptr),
    mPlayTimerScene(nullptr),
    mScenarioSelectScene(nullptr)
{
    mObjHolder = new NameObjHolder(0x1300);
    _98 = new Spine(this, &NrvGameSystemSceneController::GameSystemSceneControllerNotInitialized::sInstance);
    mIntermissionScene = static_cast<IntermissionScene*>(MR::createScene("Intermission"));
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

    if (_A0) {
        return;
    }

    mSceneInitializeState = State_NotInit;

    requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance);
}

void GameSystemSceneController::checkRequestAndChangeScene() {
    if (_9C != nullptr) {
        _98->setNerve(_9C);
        _9C = nullptr;
    }

    _98->update();
}

void GameSystemSceneController::initializeScene() {
    mSceneInitializeState = State_Init;

    if (SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap == nullptr
        || !isSameAtNextSceneAndStage())
    {
        SingletonHolder<HeapMemoryWatcher>::get()->createFileCacheHeapOnGameHeap(0x1040400);
    }

    SingletonHolder<HeapMemoryWatcher>::get()->createSceneHeapOnGameHeap();
    SingletonHolder<HeapMemoryWatcher>::get()->setCurrentHeapToSceneHeap();
    updateSceneControlInfo();
    MR::setRandomSeedFromStageName();

    if (mScenarioSelectScene->isActive()) {
        mScenarioSelectScene->validateScenarioSelect();
    }

    MR::resetWPad();

    mScene = MR::createScene(_0.mScene);
    mScene->initNameObjListExecutor();
    mScene->initSceneObjHolder();
    mScene->init();

    SceneFunction::allocateDrawBufferActorList();
    MR::clearFileLoaderRequestFileInfo(_A0);
    SingletonHolder<HeapMemoryWatcher>::get()->checkRestMemory();
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
        SingletonHolder<HeapMemoryWatcher>::get()->destroyGameHeap();
    }
    else {
        SingletonHolder<HeapMemoryWatcher>::get()->destroySceneHeap();
    }
}

bool GameSystemSceneController::isStopSound() const {
    if (MR::isEqualSceneName("Title")) {
        return false;
    }

    if (MR::isEqualStageName("AstroDome")
        && MR::isEqualString(_4C.mScene, "Game"))
    {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy")
        && MR::isEqualString(_4C.mScene, "Game")
        && MR::isEqualString(_4C.mStage, "AstroDome"))
    {
        return false;
    }

    if (MR::isEqualStageName("LibraryRoom")
        && MR::isEqualString(_4C.mScene, "Game"))
    {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy")
        && MR::isEqualString(_4C.mScene, "Game")
        && MR::isEqualString(_4C.mStage, "LibraryRoom"))
    {
        return false;
    }

    if (MR::isEqualStageName("HeavensDoorGalaxy")
        && MR::isEqualString(_4C.mScene, "Game"))
    {
        return false;
    }

    if (MR::isEqualStageName("AstroGalaxy")
        && MR::isEqualString(_4C.mScene, "Game")
        && MR::isEqualString(_4C.mStage, "HeavensDoorGalaxy"))
    {
        return false;
    }

    return true;
}

bool GameSystemSceneController::isReadyToStartScene() const {
    // FIXME: Probably an inline of `Spine::isNerve`
    return !(_98->getCurrentNerve() == &NrvGameSystemSceneController::GameSystemSceneControllerReadyToStartScene::sInstance)
        && !mScenarioSelectScene->isScenarioSelecting();
}

bool GameSystemSceneController::isFirstUpdateSceneNerveNormal() const {
    // FIXME: Probably an inline of `Spine::isNerve`
    return !(_98->getCurrentNerve() == &NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance)
        && _98->mStep == 1;
}

void GameSystemSceneController::startScene() {
    mScene->start();
    GameSystemFunction::restartControllerLeaveWatcher();
    requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance);
}

void GameSystemSceneController::updateScene() {
    if (_A0) {
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
    if (_A0) {
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
    return mScene != nullptr
        && mScene->mSceneObjHolder != nullptr;
}

s32 GameSystemSceneController::getCurrentScenarioNo() const {
    return _0.mCurrentScenarioNo;
}

s32 GameSystemSceneController::getCurrentSelectedScenarioNo() const {
    return _0.mCurrentSelectedScenarioNo;
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

void GameSystemSceneController::setCurrentScenarioNo(s32 current, s32 currentSelected) {
    _0.mCurrentScenarioNo = current;
    _0.mCurrentSelectedScenarioNo = currentSelected;
}

void GameSystemSceneController::resetCurrentScenarioNo() {
    _0.mCurrentScenarioNo = -1;
}

// GameSystemSceneController::isScenarioDecided

void GameSystemSceneController::loadScenarioWaveData() {
    getAudioSystemWrapper()->loadScenarioWaveData(_0.mScene, _0.mStage, _0.mCurrentScenarioNo);
}

bool GameSystemSceneController::isLoadDoneScenarioWaveData() const {
    return getAudioSystemWrapper()->isLoadDoneScenarioWaveData();
}

void GameSystemSceneController::exeNotInitialized() {
    if (_98->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[system setup]");
    }
}

void GameSystemSceneController::exeNormal() {

}

void GameSystemSceneController::exeWaitDrawDoneScene() {
    const Nerve* pNerve;

    bool isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        if (mScene != nullptr) {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyScene::sInstance;
        }
        else {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance;
        }
    }
    else if (mScene != nullptr) {
        pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance;
    }
    else {
        pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
    }

    requestChangeNerve(pNerve);
}

void GameSystemSceneController::exeDestroyScene() {
    if (_98->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[destroy Scene: %s]", _0.mScene);
        MR::startFunctionAsyncExecute(
            MR::Functor(this, &GameSystemSceneController::destroyScene),
            17,
            "シーン破棄");
    }

    if (MR::tryEndFunctionAsyncExecute("シーン破棄")) {
        const Nerve* pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance;
        bool isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance
            == _98->getCurrentNerve();

        if (isNerve) {
            pNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
        }

        requestChangeNerve(pNerve);
    }
}

void GameSystemSceneController::exeChangeWaveBank() {
    if (_98->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[change Wave Data]");

        bool isPlayerLuigi = GameSystemFunction::isCreatedGameDataHolder()
            ? MR::isPlayerLuigi()
            : false;

        getAudioSystemWrapper()->loadStageWaveData(_4C.mScene, _4C.mStage, isPlayerLuigi);
    }

    if (getAudioSystemWrapper()->isLoadDoneStageWaveData()) {
        requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance);
    }
}

void GameSystemSceneController::exeInitializeScene() {
    if (_98->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[initialize Scene: %s]", _4C.mScene);
        MR::startFunctionAsyncExecute(
            MR::Functor(this, &GameSystemSceneController::initializeScene),
            17,
            "シーン初期化");
    }

    if (MR::tryEndFunctionAsyncExecute("シーン初期化")) {
        mSceneInitializeState = State_End;

        requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerInvalidateSystemWipe::sInstance);
    }
}

// GameSystemSceneController::exeInvalidateSystemWipe

void GameSystemSceneController::exeDestroyed() {
    if (_98->mStep == 0) {
        mIntermissionScene->setCurrentSceneControllerState("[empty scene]");
    }
}

void GameSystemSceneController::exeReadyToStartScene() {
    if (_A0) {
        const Nerve* pNerve = getNextNerveOnResetProcessing();

        if (pNerve != nullptr) {
            requestChangeNerve(pNerve);
        }
    }
}

void GameSystemSceneController::prepareReset() {
    _A0 = true;

    mPlayTimerScene->stop();
}

// GameSystemSceneController::isPreparedReset

void GameSystemSceneController::restartGameAfterResetting() {
    tryDestroyFileCacheHeap(true);
    SingletonHolder<HeapMemoryWatcher>::get()->destroyGameHeap();

    _A0 = false;
}

// GameSystemSceneController::isExistRequest

bool GameSystemSceneController::isSameAtNextSceneAndStage() const {
    return MR::isEqualString(_0.mScene, _4C.mScene)
        && MR::isEqualString(_0.mStage, _4C.mStage)
        && _0.mCurrentScenarioNo == _4C.mCurrentScenarioNo;
}

void GameSystemSceneController::updateSceneControlInfo() {
    _0.setScene(_4C.mScene);
    _0.setStage(_4C.mStage);
    _0.mCurrentScenarioNo = _4C.mCurrentScenarioNo;
    _0.mCurrentSelectedScenarioNo = _4C.mCurrentSelectedScenarioNo;
    _0.setStartIdInfo(*_4C.mStartIdInfo);

    _4C.mScene[0] = '\0';
    _4C.mStage[0] = '\0';
    _4C.mCurrentScenarioNo = -1;
    _4C.mCurrentSelectedScenarioNo = -1;
}

Scene* GameSystemSceneController::getCurrentSceneForExecute() const {
    if (mScene != nullptr) {
        bool isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance == _98->getCurrentNerve();

        if (isNerve) {
            return mScene;
        }
    }

    return mIntermissionScene;
}

const Nerve* GameSystemSceneController::getNextNerveOnResetProcessing() const {
    bool isNerve;

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneSceneForDestroy::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroySceneForDestroy::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerDestroyScene::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerChangeWaveBank::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerNotInitialized::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return &NrvGameSystemSceneController::GameSystemSceneControllerDestroyed::sInstance;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerInitializeScene::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerInvalidateSystemWipe::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    isNerve = &NrvGameSystemSceneController::GameSystemSceneControllerReadyToStartScene::sInstance
        == _98->getCurrentNerve();

    if (isNerve) {
        return nullptr;
    }

    return &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneSceneForDestroy::sInstance;
}

bool GameSystemSceneController::tryDestroyFileCacheHeap(bool param1) {
    if (!param1
        && SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap != nullptr
        && isSameAtNextSceneAndStage())
    {
        return false;
    }

    MR::removeResourceAndFileHolderIfIsEqualHeap(
        SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap);

    return true;
}

void GameSystemSceneController::requestChangeNerve(const Nerve* pNerve) {
    const Nerve* pNextNerve;

    if (_A0) {
        pNextNerve = getNextNerveOnResetProcessing();
    }
    else {
        pNextNerve = nullptr;
    }

    if (pNextNerve == nullptr) {
        pNextNerve = pNerve;
    }

    _9C = pNextNerve;
}

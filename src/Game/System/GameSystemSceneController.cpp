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
    AudSystemWrapper* getAudioSystemWrapper() {
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
    mInitState(State_NotInit),
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

    mInitState = State_NotInit;

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
    mInitState = State_Init;

    if (!SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap != nullptr
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

// GameSystemSceneController::isStopSound
// GameSystemSceneController::isReadyToStartScene
// GameSystemSceneController::isFirstUpdateSceneNerveNormal

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

void GameSystemSceneController::setSceneInitializeState(SceneInitializeState initState) {
    mInitState = initState;
}

bool GameSystemSceneController::isSceneInitializeState(SceneInitializeState initState) const {
    return mInitState == initState;
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
// GameSystemSceneController::loadScenarioWaveData
// GameSystemSceneController::isLoadDoneScenarioWaveData
// GameSystemSceneController::exeNotInitialized

void GameSystemSceneController::exeNormal() {

}

// GameSystemSceneController::exeWaitDrawDoneScene
// GameSystemSceneController::exeDestroyScene
// GameSystemSceneController::exeChangeWaveBank
// GameSystemSceneController::exeInitializeScene
// GameSystemSceneController::exeInvalidateSystemWipe
// GameSystemSceneController::exeDestroyed
// GameSystemSceneController::exeReadyToStartScene
// GameSystemSceneController::prepareReset
// GameSystemSceneController::isPreparedReset
// GameSystemSceneController::restartGameAfterResetting
// GameSystemSceneController::isExistRequest
// GameSystemSceneController::isSameAtNextSceneAndStage
// GameSystemSceneController::updateSceneControlInfo
// GameSystemSceneController::getCurrentSceneForExecute
// GameSystemSceneController::getNextNerveOnResetProcessing
// GameSystemSceneController::tryDestroyFileCacheHeap
// GameSystemSceneController::requestChangeNerve

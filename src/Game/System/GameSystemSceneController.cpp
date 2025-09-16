#include "Game/System/GameSystemSceneController.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Scene/ScenarioSelectScene.hpp"
#include "Game/Scene/SceneFactory.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/System/ScenarioDataParser.hpp"
#include "Game/System/WPadHolder.hpp"
#include "Game/Util.hpp"
#include <cstdio>

SceneControlInfo::SceneControlInfo() {
    _48 = nullptr;
    mScene[0] = 0;
    mStage[0] = 0;
    mCurrentScenarioNo = 1;
    mCurrentSelectedScenarioNo = 1;
    _48 = new JMapIdInfo(MR::getInitializeStartIdInfo());
    
}

void SceneControlInfo::setScene(const char *pSceneName) {
    snprintf(mScene, sizeof(mScene), "%s", pSceneName);
}

void SceneControlInfo::setStage(const char *pStageName) {
    snprintf(mStage, sizeof(mStage), "%s", pStageName);
}

void SceneControlInfo::setStartIdInfo(const JMapIdInfo &rInfo) {
    *_48 = rInfo;
} 

GameSystemSceneController::GameSystemSceneController()
    : _98(nullptr), _9C(nullptr), _A0(nullptr), mScenarioParser(nullptr), mObjHolder(nullptr),
      mGameScene(nullptr), mInitState(), mIntermissionScene(nullptr), mPlayTimerScene(nullptr),
      mScenarioScene(nullptr) {
    mObjHolder = new NameObjHolder(0x1300);
    _98 = new Spine(
        this, &NrvGameSystemSceneController::GameSystemSceneControllerNotInitialized::sInstance

    );
    extern const char lbl_805DB6E0[]; // = "Intermission";
    mIntermissionScene = static_cast<IntermissionScene*>(MR::createScene(lbl_805DB6E0));
    mPlayTimerScene = new PlayTimerScene();
    mScenarioScene = new ScenarioSelectScene();
}

extern const char lbl_805DB6ED[];
void GameSystemSceneController::initAfterStationedResourceLoaded() {
    mScenarioParser = new ScenarioDataParser(lbl_805DB6ED);
    mScenarioParser->initWithoutIter();
    mPlayTimerScene->init();
    mScenarioScene->init();
}

void GameSystemSceneController::requestChangeScene() {
    if (!isExistRequest() && !_A0) {
        mInitState = State_ChangeScene;
        requestChangeNerve(
            &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance);
    }
}

void GameSystemSceneController::checkRequestAndChangeScene() {
    if (_9C) {
        _98->setNerve(_9C);
        _9C = nullptr;
    }
    _98->update();
}

void GameSystemSceneController::initializeScene() {
    mInitState = State_InitScene;
    if (!SingletonHolder<HeapMemoryWatcher>::get()->mFileCacheHeap ||
        !isSameAtNextSceneAndStage()) {
        SingletonHolder<HeapMemoryWatcher>::get()->createFileCacheHeapOnGameHeap(0x1040400);
    }
    SingletonHolder<HeapMemoryWatcher>::get()->createSceneHeapOnGameHeap();
    SingletonHolder<HeapMemoryWatcher>::get()->setCurrentHeapToSceneHeap();
    updateSceneControlInfo();
    MR::setRandomSeedFromStageName();
    if (mScenarioScene->isActive()) {
        mScenarioScene->validateScenarioSelect();
    }
    MR::resetWPad();
    mGameScene = static_cast<GameScene*>(MR::createScene(_0.mScene));
    mGameScene->initNameObjListExecutor();
    mGameScene->initSceneObjHolder();
    mGameScene->init();
    SceneFunction::allocateDrawBufferActorList();
    MR::clearFileLoaderRequestFileInfo(_A0);
    SingletonHolder<HeapMemoryWatcher>::get()->checkRestMemory();
}

void GameSystemSceneController::destroyScene() {
    bool stopSound = isStopSound();

    GameScene* gameScene = mGameScene;
    mGameScene = nullptr;
    delete gameScene;

    mObjHolder->clearArray();
    if (stopSound) {
        MR::stopAllSound(0);
    }
    GameSystemFunction::resetAllControllerRumble();
    MR::removeResourceAndFileHolderIfIsEqualHeap(MR::getSceneHeapNapa());
    if (tryDestroyFileCacheHeap(false)) {
        SingletonHolder<HeapMemoryWatcher>::get()->destroyGameHeap();
    } else {
        SingletonHolder<HeapMemoryWatcher>::get()->destroySceneHeap();
    }
}

void GameSystemSceneController::startScene() {
    mGameScene->start();
    GameSystemFunction::restartControllerLeaveWatcher();
    requestChangeNerve(&NrvGameSystemSceneController::GameSystemSceneControllerNormal::sInstance);
}

#include "Game/System/GameSystemSceneController.hpp"
#include "Game/NameObj/NameObjHolder.hpp"
#include "Game/Scene/IntermissionScene.hpp"
#include "Game/Scene/PlayTimerScene.hpp"
#include "Game/Scene/ScenarioSelectScene.hpp"
#include "Game/Scene/SceneFactory.hpp"
#include "Game/System/ScenarioDataParser.hpp"
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
        mInitState = State_InitScene;
        requestChangeNerve(
            &NrvGameSystemSceneController::GameSystemSceneControllerWaitDrawDoneScene::sInstance);
    }
}

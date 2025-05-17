#include "Game/Scene/GameScene.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"

namespace {
    GameScene* getGameScene() NO_INLINE {
        return SingletonHolder<GameSystem>::get()->mSceneController->mGameScene;
    }
};

namespace GameSceneFunction {
    void notifyEndScenarioStarter() {
        getGameScene()->notifyEndScenarioStarter();
    }

    void requestPlayMovieDemo() {
        getGameScene()->requestPlayMovieDemo();
    }

    void requestStartGameOverDemo() {
        getGameScene()->requestStartGameOverDemo();
    }

    void requestEndGameOverDemo() {
        getGameScene()->requestEndGameOverDemo();
    }

    void requestEndMissDemo() {
        getGameScene()->requestEndMissDemo();
    }

    void requestPowerStarGetDemo() {
        getGameScene()->requestPowerStarGetDemo();
    }

    void requestGrandStarGetDemo() {
        getGameScene()->requestGrandStarGetDemo();
    }

    void requestShowGalaxyMap() {
        getGameScene()->requestShowGalaxyMap();
    }

    void requestStaffRoll() {
        getGameScene()->requestStaffRoll();
    }

    bool isExecScenarioOpeningCamera() {
        return getGameScene()->isExecScenarioOpeningCamera();
    }

    bool isExecScenarioStarter() {
        return getGameScene()->isExecScenarioStarter();
    }

    bool isExecStageClearDemo() {
        return getGameScene()->isExecStageClearDemo();
    }

    void loadScenarioWaveData() {
        SingletonHolder<GameSystem>::get()->mSceneController->loadScenarioWaveData();
    }

    bool isLoadDoneScenarioWaveData() {
        return SingletonHolder<GameSystem>::get()->mSceneController->isLoadDoneScenarioWaveData();
    }

    void activateDraw3D() {
        GameScene* pGameScene = getGameScene();

        pGameScene->mDraw3D = true;
    }

    void deactivateDraw3D() {
        GameScene* pGameScene = getGameScene();

        pGameScene->mDraw3D = false;
    }
};

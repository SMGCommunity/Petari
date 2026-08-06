#include "Game/Util/SequenceUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/SystemUtil.hpp"

namespace MR {
    void requestChangeScene(const char* pName) {
        GameSequenceFunction::requestChangeScene(pName);
    }

    void requestChangeSceneTitle() {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(7, nullptr, 1, nullptr));
    }

    void requestChangeStageInGameAfterLoadingGameData() {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(6, nullptr, 1, nullptr));
    }

    void requestChangeStageAfterStageClear() {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(4, nullptr, 1, nullptr));
    }

    void requestChangeStageAfterMiss() {
        JMapIdInfo info = *getPlayerRestartIdInfo();

        if (isGalaxyAnyCometAppearInCurrentStage()) {
            // FIXME: `JMapIdInfo::operator=` is not getting inlined here.
            info = getInitializeStartIdInfo();
        }

        GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;
        GalaxyMoveArgument argument(5, pSceneController->mCurrSceneControlInfo.mStage, pSceneController->getCurrentScenarioNo(), &info);
        argument._C = pSceneController->getCurrentSelectedScenarioNo();

        GameSequenceFunction::requestGalaxyMove(argument);
    }

    void requestChangeStageInGameMoving(const char* pStageName, s32 scenarioNo, const JMapIdInfo& rInfo) {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, pStageName, scenarioNo, &rInfo));
    }

    void requestChangeStageInGameMoving(const char* pStageName, s32 scenarioNo) {
        requestChangeStageInGameMoving(pStageName, scenarioNo, getInitializeStartIdInfo());
    }

    void requestChangeSceneAfterGameOver() {
        resetSystemAndGameStatus();
        requestChangeSceneTitle();
    }

    void requestChangeSceneAfterBoot() {
        requestChangeSceneTitle();
    }

    void requestChangeStageGoBackAstroDome() {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(3, nullptr, 1, nullptr));
    }

    void requestStartScenarioSelect(const char* pStageName) {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(2, pStageName, -1, nullptr));
    }

    void requestStartScenarioSelectForComet(const char* pStageName, s32 scenarioNo) {
        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(2, pStageName, scenarioNo, nullptr));
    }

    bool hasRetryGalaxySequence() {
        return GameSequenceFunction::hasRetryGalaxySequence();
    }

    bool isExecScenarioStarter() {
        return GameSceneFunction::isExecScenarioStarter();
    }

    void requestPowerStarGetDemo() {
        GameSceneFunction::requestPowerStarGetDemo();
    }

    void requestGrandStarGetDemo() {
        GameSceneFunction::requestGrandStarGetDemo();
    }

    void requestStartGameOverDemo() {
        GameSceneFunction::requestStartGameOverDemo();
    }

    void requestEndGameOverDemo() {
        GameSceneFunction::requestEndGameOverDemo();
    }

    void requestEndMissDemo() {
        GameSceneFunction::requestEndMissDemo();
    }

    void requestShowGalaxyMap() {
        GameSceneFunction::requestShowGalaxyMap();
    }

    void executeOnWelcomeAndRetry() {
        startGalaxyCometEvent();
        requestForceAppearHPMeter();
    }

    void requestGoToAstroGalaxy(s32 param1) {
        JMapIdInfo info(param1, 0);

        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, "AstroGalaxy", -1, &info));
    }

    void requestGoToAstroDomeFromAstroGalaxy(s32 scenarioNo, s32 param2) {
        JMapIdInfo info(param2, 0);

        GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, "AstroDome", scenarioNo, &info));
    }
};  // namespace MR

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

void MR::requestChangeScene(const char* pName) {
    GameSequenceFunction::requestChangeScene(pName);
}

void MR::requestChangeSceneTitle() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(7, nullptr, 1, nullptr));
}

void MR::requestChangeStageInGameAfterLoadingGameData() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(6, nullptr, 1, nullptr));
}

void MR::requestChangeStageAfterStageClear() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(4, nullptr, 1, nullptr));
}

void MR::requestChangeStageAfterMiss() {
    JMapIdInfo info = *getPlayerRestartIdInfo();

    if (isGalaxyAnyCometAppearInCurrentStage()) {
        const JMapIdInfo& rInitialInfo = getInitializeStartIdInfo();
        info._0 = rInitialInfo._0;
        info.mZoneID = rInitialInfo.mZoneID;
    }

    GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;
    GalaxyMoveArgument argument(5, pSceneController->mCurrSceneControlInfo.mStage, pSceneController->getCurrentScenarioNo(), &info);
    argument._C = pSceneController->getCurrentSelectedScenarioNo();

    GameSequenceFunction::requestGalaxyMove(argument);
}

void MR::requestChangeStageInGameMoving(const char* pStageName, s32 scenarioNo, const JMapIdInfo& rInfo) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, pStageName, scenarioNo, &rInfo));
}

void MR::requestChangeStageInGameMoving(const char* pStageName, s32 scenarioNo) {
    requestChangeStageInGameMoving(pStageName, scenarioNo, getInitializeStartIdInfo());
}

void MR::requestChangeSceneAfterGameOver() {
    resetSystemAndGameStatus();
    requestChangeSceneTitle();
}

void MR::requestChangeSceneAfterBoot() {
    requestChangeSceneTitle();
}

void MR::requestChangeStageGoBackAstroDome() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(3, nullptr, 1, nullptr));
}

void MR::requestStartScenarioSelect(const char* pStageName) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(2, pStageName, -1, nullptr));
}

void MR::requestStartScenarioSelectForComet(const char* pStageName, s32 scenarioNo) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(2, pStageName, scenarioNo, nullptr));
}

bool MR::hasRetryGalaxySequence() {
    return GameSequenceFunction::hasRetryGalaxySequence();
}

bool MR::isExecScenarioStarter() {
    return GameSceneFunction::isExecScenarioStarter();
}

void MR::requestPowerStarGetDemo() {
    GameSceneFunction::requestPowerStarGetDemo();
}

void MR::requestGrandStarGetDemo() {
    GameSceneFunction::requestGrandStarGetDemo();
}

void MR::requestStartGameOverDemo() {
    GameSceneFunction::requestStartGameOverDemo();
}

void MR::requestEndGameOverDemo() {
    GameSceneFunction::requestEndGameOverDemo();
}

void MR::requestEndMissDemo() {
    GameSceneFunction::requestEndMissDemo();
}

void MR::requestShowGalaxyMap() {
    GameSceneFunction::requestShowGalaxyMap();
}

void MR::executeOnWelcomeAndRetry() {
    startGalaxyCometEvent();
    requestForceAppearHPMeter();
}

void MR::requestGoToAstroGalaxy(s32 param1) {
    JMapIdInfo info(param1, 0);

    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, "AstroGalaxy", -1, &info));
}

void MR::requestGoToAstroDomeFromAstroGalaxy(s32 scenarioNo, s32 param2) {
    JMapIdInfo info(param2, 0);

    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(0, "AstroDome", scenarioNo, &info));
}

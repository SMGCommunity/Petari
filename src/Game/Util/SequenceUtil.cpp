#include "Game/Util/SequenceUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/Util/SystemUtil.hpp"

void MR::requestChangeScene(const char* pName) {
    GameSequenceFunction::requestChangeScene(pName);
}

void MR::requestChangeSceneTitle() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_TITLE, nullptr, 1, nullptr));
}

void MR::requestChangeStageInGameAfterLoadingGameData() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_AFTER_LOADING_GAME_DATA, nullptr, 1, nullptr));
}

void MR::requestChangeStageAfterStageClear() {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_AFTER_CLEAR, nullptr, 1, nullptr));
}

void MR::requestChangeStageAfterMiss() {
    JMapIdInfo info = *getPlayerRestartIdInfo();

    if (isGalaxyAnyCometAppearInCurrentStage()) {
        const JMapIdInfo& rInitialInfo = getInitializeStartIdInfo();
        info.mID = rInitialInfo.mID;
        info.mZoneID = rInitialInfo.mZoneID;
    }

    GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;
    GalaxyMoveArgument argument(GALAXY_MOVE_AFTER_MISS, pSceneController->mCurrSceneControlInfo.mStage, pSceneController->getCurrentScenarioNo(),
                                &info);
    argument.mSelectedScenarioNo = pSceneController->getCurrentSelectedScenarioNo();

    GameSequenceFunction::requestGalaxyMove(argument);
}

void MR::requestChangeStageInGameMoving(const char* pStageName, s32 scenarioNo, const JMapIdInfo& rInfo) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_MOVING, pStageName, scenarioNo, &rInfo));
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
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_BACK_ASTRO_DOME, nullptr, 1, nullptr));
}

void MR::requestStartScenarioSelect(const char* pStageName) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_SCENARIO_SELECT, pStageName, -1, nullptr));
}

void MR::requestStartScenarioSelectForComet(const char* pStageName, s32 scenarioNo) {
    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_SCENARIO_SELECT, pStageName, scenarioNo, nullptr));
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

    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_MOVING, "AstroGalaxy", -1, &info));
}

void MR::requestGoToAstroDomeFromAstroGalaxy(s32 scenarioNo, s32 param2) {
    JMapIdInfo info(param2, 0);

    GameSequenceFunction::requestGalaxyMove(GalaxyMoveArgument(GALAXY_MOVE_MOVING, "AstroDome", scenarioNo, &info));
}

bool MR::isProductSequence() {
    return true;
}

bool MR::isMonitorPlaySequence() {
    return false;
}

void MR::requestChangeStageInGameWithSyncGameData(const char* pStageName, s32 scenarioNo) {
    GameDataFunction::resetSequenceGameData();

    if (MR::isEqualString(pStageName, "AstroGalaxy")) {
        switch (scenarioNo) {
        case 1:
            GameDataFunction::tryGetPowerStarToGetGrandStar(1);
            break;
        case 2:
            GameDataFunction::tryGetPowerStarToGetGrandStar(3);
            break;
        case 3:
            GameDataFunction::tryGetPowerStarToGetGrandStar(5);
            break;
        case 4:
            GameDataFunction::tryGetPowerStarToGetGrandStar(1);
            break;
        case 5:
            GameDataFunction::tryGetPowerStarToOpenGalaxyAndScenario("KoopaBattleVs3Galaxy", 1);
            break;
        }
    } else {
        if (MR::isEqualString(pStageName, "AstroDome")) {
            if (scenarioNo - 1u <= 5) {
                GameDataFunction::tryGetPowerStarToGetGrandStar(scenarioNo);
            }
        } else if (MR::isEqualString(pStageName, "HeavensDoorGalaxy") && scenarioNo == 2) {
            GameDataFunction::tryGetPowerStarToGetGrandStar(6);
        } else {
            GameDataFunction::tryGetPowerStarToOpenGalaxyAndScenario(pStageName, scenarioNo);
        }
    }

    s32 selectedScenarioNo = -1;

    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pStageName);
    if (accessor.isHiddenStar(scenarioNo)) {
        selectedScenarioNo = scenarioNo;
        scenarioNo = MR::getPlacedHiddenStarScenarioNo(pStageName, scenarioNo);
    }

    if (MR::isEqualString("FactoryGalaxy", pStageName) && scenarioNo == 4) {
        int starPieceNum = MR::getStarPieceNumMaxGivingToTicoSeed(4);
        MR::addStarPieceGivingToTicoSeed(4, starPieceNum);
    }

    GalaxyMoveArgument moveArg(GALAXY_MOVE_SYNC_GAME_DATA, pStageName, scenarioNo, nullptr);
    moveArg.mSelectedScenarioNo = selectedScenarioNo;
    GameSequenceFunction::requestGalaxyMove(moveArg);
}

#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/Scene/PlacementStateChecker.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"

namespace {
    ScenePlayingResult* getScenePlayingResult() {
        return MR::getSceneObj< ScenePlayingResult >(SceneObj_ScenePlayingResult);
    }
}; // namespace

namespace MR {
    s32 getCurrentScenarioNo() {
        return SingletonHolder< GameSystem >::get()->mSceneController->getCurrentScenarioNo();
    }

    s32 getCurrentSelectedScenarioNo() {
        return SingletonHolder< GameSystem >::get()->mSceneController->getCurrentSelectedScenarioNo();
    }

    void setCurrentScenarioNo(s32 current, s32 currentSelected) {
        return SingletonHolder< GameSystem >::get()->mSceneController->setCurrentScenarioNo(current, currentSelected);
    }

    bool isScenarioDecided() {
        return SingletonHolder< GameSystem >::get()->mSceneController->isScenarioDecided();
    }

    const char* getCurrentStageName() {
        return SingletonHolder< GameSystem >::get()->mSceneController->_0.mStage;
    }

    bool isEqualSceneName(const char* pSceneName) {
        return isEqualStringCase(SingletonHolder< GameSystem >::get()->mSceneController->_0.mScene, pSceneName);
    }

    bool isEqualStageName(const char* pStageName) {
        const char* pStage = SingletonHolder< GameSystem >::get()->mSceneController->_0.mStage;

        if (pStage == nullptr) {
            return false;
        }

        return isEqualStringCase(pStage, pStageName);
    }

    bool isStageBeginPrologueEvent() {
        return isEqualStageName("PeachCastleGardenGalaxy");
    }

    bool isStageBeginFadeWipe() {
        return isEqualStageName("HeavensDoorGalaxy");
    }

    bool isStageBeginTitleWipe() {
        return isStageFileSelect();
    }

    bool isStageBeginWithoutWipe() {
        return isStageEpilogueDemo();
    }

    bool isStageDisablePauseMenu() {
        return isStageFileSelect() || isStageEpilogueDemo();
    }

    bool isStageAstroLocation() {
        return isEqualStageName("AstroGalaxy") || isEqualStageName("AstroDome") || isEqualStageName("LibraryRoom");
    }

    bool isStageSwimAngleLimit() {
        return isEqualStageName("OceanRingGalaxy");
    }

    bool isStageStarPieceFollowGroupLimit() {
        return isEqualStageName("EggStarGalaxy") && getCurrentScenarioNo() == 2;
    }

    bool isStageFileSelect() {
        return isEqualStageName("FileSelect");
    }

    bool isStageKoopaVs() {
        return isStageKoopaVs1() || isStageKoopaVs2() || isStageKoopaVs3();
    }

    bool isStageKoopaVs1() {
        return isEqualStageName("KoopaBattleVs3Galaxy");
    }

    bool isStageKoopaVs2() {
        return isEqualStageName("KoopaBattleVs2Galaxy");
    }

    bool isStageKoopaVs3() {
        return isEqualStageName("KoopaBattleVs3Galaxy");
    }

    bool isStageEpilogueDemo() {
        return isEqualStageName("EpilogueDemoStage");
    }

    bool isBeginScenarioStarter() {
        if (hasRetryGalaxySequence()) {
            return false;
        }

        return NameObjFinder::find("シナリオスターター");
    }

    bool isStageSuddenDeathDodoryu() {
        return isEqualStageName("CosmosGardenGalaxy") && getCurrentScenarioNo() == 4;
    }

    void setInitializeStatePlacementPlayer() {
        SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(State_PlacementPlayer);
    }

    void setInitializeStatePlacementHighPriority() {
        SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(State_PlacementHighPrio);
    }

    void setInitializeStatePlacement() {
        SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(State_Placement);
    }

    void setInitializeStateAfterPlacement() {
        SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(State_AfterPlacement);
    }

    bool isInitializeStateEnd() {
        return SingletonHolder< GameSystem >::get()->mSceneController->isSceneInitializeState(State_End);
    }

    // isInitializeStatePlacementSomething
    // stopSceneForScenarioOpeningCamera
    // playSceneForScenarioOpeningCamera
    // getCurrentMarioStartIdInfo
    // getStartPosNum
    // getCurrentStartZoneId
    // getInitializeStartIdInfo
    // getStageArchive
    // getGeneralPosNum
    // getGeneralPosData
    // getChildObjNum
    // getChildObjName
    // initChildObj

    const char* getAppearPowerStarObjName(s32 scenarioNo) {
        return makeCurrentGalaxyStatusAccessor().getAppearPowerStarObjName(scenarioNo);
    }

    s32 getCurrentStageNormalScenarioNum() {
        return makeCurrentGalaxyStatusAccessor().getNormalScenarioNum();
    }

    s32 getCurrentStagePowerStarNum() {
        return makeCurrentGalaxyStatusAccessor().getPowerStarNum();
    }

    s32 getZoneNum() {
        return makeCurrentGalaxyStatusAccessor().getZoneNum();
    }

    const char* getZoneNameFromZoneId(s32 zoneId) {
        return makeCurrentGalaxyStatusAccessor().getZoneName(zoneId);
    }

    // getPlacedHiddenStarScenarioNo
    // getRailInfo
    // getNextLinkRailInfo
    // getCurrentStartCameraId
    // getStartCameraIdInfoFromStartDataIndex
    // getPlacedRailNum
    // getCameraRailInfo
    // getCameraRailInfoFromRailDataIndex
    // getStageCameraData
    // getCurrentScenarioStartAnimCameraData

    void incCoin(int term) {
        getScenePlayingResult()->incCoin(term);
        incPlayerLife(term);
    }

    void incPurpleCoin() {
        getScenePlayingResult()->incPurpleCoin();
    }

    s32 getCoinNum() {
        return getScenePlayingResult()->getCoinNum();
    }

    s32 getPurpleCoinNum() {
        return getScenePlayingResult()->mPurpleCoinNum;
    }

    s32 getPowerStarNum() {
        return GameDataFunction::calcCurrentPowerStarNum();
    }

    // isPlacementLocalStage
    // getPlacedZoneId
    // getZonePlacementMtx
    // getZonePlacementMtx
    // getJapaneseObjectName

    void setCurrentPlacementZoneId(s32 zoneId) {
        getPlacementStateChecker()->setCurrentPlacementZoneId(zoneId);
    }

    void clearCurrentPlacementZoneId() {
        getPlacementStateChecker()->clearCurrentPlacementZoneId();
    }

    s32 getCurrentPlacementZoneId() {
        return getPlacementStateChecker()->getCurrentPlacementZoneId();
    }

    const char* getCurrentPlacementZoneName() {
        return getZoneNameFromZoneId(getCurrentPlacementZoneId());
    }
}; // namespace MR

#include "Game/Util/SceneUtil.hpp"
#include "Game/NameObj/NameObjFinder.hpp"
#include "Game/Scene/PlacementStateChecker.hpp"
#include "Game/Scene/SceneNameObjMovementController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/Scene/StageDataHolder.hpp"
#include "Game/Scene/StageFileLoader.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/PlacedHiddenStarScenarioTable.hpp"
#include "Game/Util/FileUtil.hpp"
#include "Game/Util/JMapIdInfo.hpp"
#include "Game/Util/JMapLinkInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    const JMapIdInfo cInitializeStartIdInfo(0, 0);

    void getRailInfoFromRailId(JMapInfoIter* pIter, const JMapInfo** ppInfo, const StageDataHolder* pHolder, int railId) {
        *pIter = pHolder->getCommonPathPointInfo(ppInfo, railId);
    }

    ScenePlayingResult* getScenePlayingResult() {
        return MR::getSceneObj< ScenePlayingResult >(SceneObj_ScenePlayingResult);
    }

    bool isInitializeState(SceneInitializeState state) {
        return SingletonHolder< GameSystem >::get()->mSceneController->isSceneInitializeState(state);
    }
};  // namespace

s32 MR::getCurrentScenarioNo() {
    return SingletonHolder< GameSystem >::get()->mSceneController->getCurrentScenarioNo();
}

s32 MR::getCurrentSelectedScenarioNo() {
    return SingletonHolder< GameSystem >::get()->mSceneController->getCurrentSelectedScenarioNo();
}

void MR::setCurrentScenarioNo(s32 scenarioNo, s32 selectedScenarioNo) {
    return SingletonHolder< GameSystem >::get()->mSceneController->setCurrentScenarioNo(scenarioNo, selectedScenarioNo);
}

bool MR::isScenarioDecided() {
    return SingletonHolder< GameSystem >::get()->mSceneController->isScenarioDecided();
}

const char* MR::getCurrentStageName() {
    return SingletonHolder< GameSystem >::get()->mSceneController->mCurrSceneControlInfo.mStage;
}

bool MR::isEqualSceneName(const char* pSceneName) {
    return isEqualStringCase(SingletonHolder< GameSystem >::get()->mSceneController->mCurrSceneControlInfo.mScene, pSceneName);
}

bool MR::isEqualStageName(const char* pStageName) {
    const char* pStage = SingletonHolder< GameSystem >::get()->mSceneController->mCurrSceneControlInfo.mStage;

    if (pStage == nullptr) {
        return false;
    }

    return isEqualStringCase(pStage, pStageName);
}

bool MR::isStageBeginPrologueEvent() {
    return isEqualStageName("PeachCastleGardenGalaxy");
}

bool MR::isStageBeginFadeWipe() {
    return isEqualStageName("HeavensDoorGalaxy");
}

bool MR::isStageBeginTitleWipe() {
    return isStageFileSelect();
}

bool MR::isStageBeginWithoutWipe() {
    return isStageEpilogueDemo();
}

bool MR::isStageDisablePauseMenu() {
    bool disabled = false;
    if (isStageFileSelect() || isStageEpilogueDemo()) {
        disabled = true;
    }

    return disabled;
}

bool MR::isStageAstroLocation() {
    return isEqualStageName("AstroGalaxy") || isEqualStageName("AstroDome") || isEqualStageName("LibraryRoom");
}

bool MR::isStageSwimAngleLimit() {
    return isEqualStageName("OceanRingGalaxy");
}

bool MR::isStageStarPieceFollowGroupLimit() {
    return isEqualStageName("EggStarGalaxy") && getCurrentScenarioNo() == 2;
}

bool MR::isStageFileSelect() {
    return isEqualStageName("FileSelect");
}

namespace {
    bool isStageKoopaVs1Or2() {
        bool result = true;
        if (!MR::isEqualStageName("KoopaBattleVs1Galaxy") && !MR::isEqualStageName("KoopaBattleVs2Galaxy")) {
            result = false;
        }
        return result;
    }
};  // namespace

bool MR::isStageKoopaVs() {
    // FAKEMATCH: `return isStageKoopaVs1() || isStageKoopaVs2() || isStageKoopaVs3()`?
    bool isKoopaVs = true;
    bool isFirstOrSecond = isStageKoopaVs1Or2();
    if (!isFirstOrSecond && !isStageKoopaVs3()) {
        isKoopaVs = false;
    }

    return isKoopaVs;
}

bool MR::isStageKoopaVs1() {
    return isEqualStageName("KoopaBattleVs1Galaxy");
}

bool MR::isStageKoopaVs2() {
    return isEqualStageName("KoopaBattleVs2Galaxy");
}

bool MR::isStageKoopaVs3() {
    return isEqualStageName("KoopaBattleVs3Galaxy");
}

bool MR::isStageEpilogueDemo() {
    return isEqualStageName("EpilogueDemoStage");
}

bool MR::isBeginScenarioStarter() {
    if (hasRetryGalaxySequence()) {
        return false;
    }

    return NameObjFinder::find("シナリオスターター");
}

bool MR::isStageSuddenDeathDodoryu() {
    return isEqualStageName("CosmosGardenGalaxy") && getCurrentScenarioNo() == 4;
}

void MR::setInitializeStatePlacementPlayer() {
    SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(SceneInitializeState_PlacementPlayer);
}

void MR::setInitializeStatePlacementHighPriority() {
    SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(SceneInitializeState_PlacementHighPriority);
}

void MR::setInitializeStatePlacement() {
    SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(SceneInitializeState_Placement);
}

void MR::setInitializeStateAfterPlacement() {
    SingletonHolder< GameSystem >::get()->mSceneController->setSceneInitializeState(SceneInitializeState_AfterPlacement);
}

bool MR::isInitializeStateEnd() {
    return SingletonHolder< GameSystem >::get()->mSceneController->isSceneInitializeState(SceneInitializeState_End);
}

bool MR::isInitializeStatePlacementSomething() {
    return (::isInitializeState(SceneInitializeState_PlacementPlayer) || ::isInitializeState(SceneInitializeState_PlacementHighPriority)) ||
           ::isInitializeState(SceneInitializeState_Placement);
}

void MR::stopSceneForScenarioOpeningCamera() {
    getSceneNameObjMovementController()->requestStopSceneFor(MovementControlType_4, nullptr);
}

void MR::playSceneForScenarioOpeningCamera() {
    getSceneNameObjMovementController()->requestPlaySceneFor(MovementControlType_4, nullptr);
}

const JMapIdInfo& MR::getCurrentMarioStartIdInfo() {
    return *SingletonHolder< GameSystem >::get()->mSceneController->mCurrSceneControlInfo.mStartIdInfo;
}

s32 MR::getStartPosNum() {
    return getStageDataHolder()->getStartPosNum();
}

s32 MR::getCurrentStartZoneId() {
    return getStageDataHolder()->getCurrentStartZoneId();
}

const JMapIdInfo& MR::getInitializeStartIdInfo() {
    return ::cInitializeStartIdInfo;
}

JKRMemArchive* MR::getStageArchive(const char* pStageName) {
    char archiveName[256];
    StageFileLoader::makeStageArchiveName(archiveName, sizeof(archiveName), pStageName);
    return receiveArchive(archiveName);
}

s32 MR::getGeneralPosNum() {
    return getStageDataHolder()->getGeneralPosNum();
}

void MR::getGeneralPosData(const char** ppName, TVec3f* pPosition, TVec3f* pRotation, JMapLinkInfo** ppLinkInfo, int index) {
    JMapInfoIter iter = getStageDataHolder()->getGeneralPosInfoFromDataIndex(index);
    iter.getValue("PosName", ppName);
    getJMapInfoTrans(iter, pPosition);
    getJMapInfoRotate(iter, pRotation);
    *ppLinkInfo = new JMapLinkInfo(iter, false);
}

s32 MR::getChildObjNum(const JMapInfoIter& rIter) {
    return getStageDataHolder()->findPlacedStageDataHolder(rIter)->getChildObjNum(rIter);
}

void MR::getChildObjName(const char** ppName, const JMapInfoIter& rIter, int index) {
    JMapInfoIter childIter = getStageDataHolder()->findPlacedStageDataHolder(rIter)->getChildObjInfoFromDataIndex(rIter, index);
    getObjectName(ppName, childIter);
}

void MR::initChildObj(NameObj* pObj, const JMapInfoIter& rIter, int index) {
    JMapInfoIter childIter = getStageDataHolder()->findPlacedStageDataHolder(rIter)->getChildObjInfoFromDataIndex(rIter, index);
    pObj->init(childIter);
}

const char* MR::getAppearPowerStarObjName(s32 scenarioNo) {
    return makeCurrentGalaxyStatusAccessor().getAppearPowerStarObjName(scenarioNo);
}

s32 MR::getCurrentStageNormalScenarioNum() {
    return makeCurrentGalaxyStatusAccessor().getNormalScenarioNum();
}

s32 MR::getCurrentStagePowerStarNum() {
    return makeCurrentGalaxyStatusAccessor().getPowerStarNum();
}

s32 MR::getZoneNum() {
    return makeCurrentGalaxyStatusAccessor().getZoneNum();
}

const char* MR::getZoneNameFromZoneId(s32 zoneId) {
    return makeCurrentGalaxyStatusAccessor().getZoneName(zoneId);
}

s32 MR::getPlacedHiddenStarScenarioNo(const char* pStageName, s32 starId) {
    GalaxyStatusAccessor accessor = makeGalaxyStatusAccessor(pStageName);
    s32 scenarioNo = getPlacedHiddenStarScenarioNoFromTable(pStageName, starId);

    if (scenarioNo > 0) {
        return scenarioNo;
    }

    s32 mask;
    s32 i = 1;
    mask = 1 << (starId - 1);
    for (; i <= accessor.getNormalScenarioNum(); i++) {
        u32 activeStars = accessor.getActivePowerStarId(i);
        if (activeStars & mask) {
            return i;
        }
    }

    return 1;
}

bool SceneUtil_DUMMY(const JMapInfoIter& rIter) {
    return MR::isEqualRailUsage(rIter, "General");
}

void MR::getRailInfo(JMapInfoIter* pRailIter, const JMapInfo** ppPointInfo, const JMapInfoIter& rIter) {
    s32 railId = -1;
    getRailId(rIter, &railId);
    ::getRailInfoFromRailId(pRailIter, ppPointInfo, getStageDataHolder()->findPlacedStageDataHolder(rIter), railId);
}

bool MR::getNextLinkRailInfo(JMapInfoIter* pRailIter, const JMapInfo** ppPointInfo, const JMapInfoIter& rIter) {
    s32 railId;
    getNextLinkRailID(rIter, &railId);

    if (railId == -1) {
        return false;
    }

    const StageDataHolder* pHolder = getStageDataHolder()->findPlacedStageDataHolder(rIter);
    ::getRailInfoFromRailId(pRailIter, ppPointInfo, pHolder, railId);

    return true;
}

s32 MR::getCurrentStartCameraId() {
    return getStageDataHolder()->getCurrentStartCameraId();
}

void MR::getStartCameraIdInfoFromStartDataIndex(JMapIdInfo* pInfo, int index) {
    getStageDataHolder()->getStartCameraIdInfoFromStartDataIndex(pInfo, index);
}

s32 MR::getPlacedRailNum(s32 zoneId) {
    if (getStageDataHolder()->isPlacedZone(zoneId)) {
        return getStageDataHolder()->getStageDataHolderFromZoneId(zoneId)->getCommonPathInfoElementNum();
    }

    return 0;
}

void MR::getCameraRailInfo(JMapInfoIter* pRailIter, const JMapInfo** ppPointInfo, s32 railId, s32 zoneId) {
    ::getRailInfoFromRailId(pRailIter, ppPointInfo, getStageDataHolder()->getStageDataHolderFromZoneId(zoneId), railId);
}

bool MR::getCameraRailInfoFromRailDataIndex(JMapInfoIter* pRailIter, const JMapInfo** ppPointInfo, int index, s32 zoneId) {
    *pRailIter = getStageDataHolder()->getStageDataHolderFromZoneId(zoneId)->getCommonPathPointInfoFromRailDataIndex(ppPointInfo, index);
    return isEqualRailUsage(*pRailIter, "Camera");
}

void MR::getStageCameraData(void** ppData, s32* pSize, s32 zoneId) {
    if (!getStageDataHolder()->isPlacedZone(zoneId)) {
        *ppData = nullptr;
        *pSize = 0;
        return;
    }

    StageDataHolder* pHolder = getStageDataHolder()->getStageDataHolderFromZoneId(zoneId);
    *ppData = pHolder->getStageArchiveResource("CameraParam.bcam");
    *pSize = pHolder->getStageArchiveResourceSize(*ppData);
}

void MR::getCurrentScenarioStartAnimCameraData(void** ppData, s32* pSize) {
    StageDataHolder* pHolder = getStageDataHolder();
    char fileName[64];
    snprintf(fileName, sizeof(fileName), "StartScenario%d.canm", getCurrentScenarioNo());
    *ppData = pHolder->getStageArchiveResource(fileName);

    if (*ppData != nullptr) {
        *pSize = pHolder->getStageArchiveResourceSize(*ppData);
    } else {
        *pSize = 0;
    }
}

void MR::incCoin(int term) {
    ::getScenePlayingResult()->incCoin(term);
    incPlayerLife(term);
}

void MR::incPurpleCoin() {
    ::getScenePlayingResult()->incPurpleCoin();
}

s32 MR::getCoinNum() {
    return ::getScenePlayingResult()->getCoinNum();
}

s32 MR::getPurpleCoinNum() {
    return ::getScenePlayingResult()->mPurpleCoinNum;
}

s32 MR::getPowerStarNum() {
    return GameDataFunction::calcCurrentPowerStarNum();
}

bool MR::isPlacementLocalStage() {
    return getStageDataHolder()->mStageDataHolderCount > 0;
}

s32 MR::getPlacedZoneId(const JMapInfoIter& rIter) {
    return getStageDataHolder()->findPlacedStageDataHolder(rIter)->mZoneID;
}

const TPos3f* MR::getZonePlacementMtx(const JMapInfoIter& rIter) {
    return &getStageDataHolder()->findPlacedStageDataHolder(rIter)->mPlacementMtx;
}

const TPos3f* MR::getZonePlacementMtx(s32 zoneId) {
    return &getStageDataHolder()->getStageDataHolderFromZoneId(zoneId)->mPlacementMtx;
}

const char* MR::getJapaneseObjectName(const char* pName) {
    const char* pJapaneseName = getStageDataHolder()->getJapaneseObjectName(pName);

    if (pJapaneseName != nullptr) {
        return pJapaneseName;
    }

    return pName;
}

void MR::setCurrentPlacementZoneId(s32 zoneId) {
    getPlacementStateChecker()->setCurrentPlacementZoneId(zoneId);
}

void MR::clearCurrentPlacementZoneId() {
    getPlacementStateChecker()->clearCurrentPlacementZoneId();
}

s32 MR::getCurrentPlacementZoneId() {
    return getPlacementStateChecker()->getCurrentPlacementZoneId();
}

const char* MR::getCurrentPlacementZoneName() {
    return getZoneNameFromZoneId(getCurrentPlacementZoneId());
}

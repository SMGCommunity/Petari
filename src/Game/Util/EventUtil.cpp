#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/Screen/InformationObserver.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameDataFunction.hpp"
// #include "Game/System/GameDataGalaxyStorage.hpp"
#include "Game/System/GameEventFlag.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include <cstdio>

namespace {
    ScenePlayingResult* getScenePlayingResult() {
        return MR::getSceneObj<ScenePlayingResult*>(SceneObj_ScenePlayingResult);
    }
};

namespace MR {
    s32 getPlayerLeft() {
        return GameDataFunction::getPlayerLeft();
    }

    void incPlayerLeft() {
        GameDataFunction::addPlayerLeft(1);
    }

    void decPlayerLeft() {
        GameDataFunction::addPlayerLeft(-1);
        GameDataFunction::addMissPoint(1);
        GameDataFunction::incPlayerMissNum();
    }

    bool isLuigiLeftSupply() {
        return GameDataFunction::isLuigiLeftSupply();
    }

    void incPlayerGameOverNum() {
        GameDataFunction::addMissPoint(3);
        GameDataFunction::incPlayerMissNum();
    }

    // isAnyPlayerLeftSupply

    void offAllPlayerLeftSupply() {
        if (GameDataFunction::isPlayerLeftSupply()) {
            GameDataFunction::offPlayerLeftSupply();
        }

        if (GameDataFunction::isPointCollectForLetter()) {
            GameDataFunction::resetMissPoint();
        }

        if (GameDataFunction::isLuigiLeftSupply()) {
            GameDataFunction::offLuigiLeftSupply();
        }
    }

    s32 getStarPieceNum() {
        if (MR::isStageAstroLocation()) {
            return GameDataFunction::getStockedStarPieceNum();
        }

        return getScenePlayingResult()->getStarPieceNum();
    }

    void addStarPiece(int num) {
        if (MR::isStageAstroLocation()) {
            GameDataFunction::addStockedStarPiece(num);
        }
        else {
            getScenePlayingResult()->incStarPiece(num);
        }
    }

    s32 getStockedStarPieceNum() {
        return GameDataFunction::getStockedStarPieceNum();
    }

    void addStockedStarPiece(int num) {
        GameDataFunction::addStockedStarPiece(num);
    }

    bool isPlayerLuigi() {
        return !GameDataFunction::isDataMario();
    }

    void explainEnableToSpin(LiveActor* pActor) {
        InformationObserverFunction::explainSpin(pActor);
    }

    void onGameEventFlagBeeMarioAtFirst() {
        GameDataFunction::onGameEventFlag("ハチマリオ初変身");
    }

    void onGameEventFlagTeresaMarioAtFirst() {
        GameDataFunction::onGameEventFlag("テレサマリオ初変身");
    }

    void onGameEventFlagHopperMarioAtFirst() {
        GameDataFunction::onGameEventFlag("ホッパーマリオ初変身");
    }

    void onGameEventFlagFireMarioAtFirst() {
        GameDataFunction::onGameEventFlag("ファイアマリオ初変身");
    }

    void onGameEventFlagIceMarioAtFirst() {
        GameDataFunction::onGameEventFlag("アイスマリオ初変身");
    }

    void onGameEventFlagFlyingMarioAtFirst() {
        GameDataFunction::onGameEventFlag("フライングマリオ初変身");
    }

    void onGameEventFlagInvincibleMarioAtFirst() {
        GameDataFunction::onGameEventFlag("無敵マリオ初変身");
    }

    void onGameEventFlagLifeUpAtFirst() {
        GameDataFunction::onGameEventFlag("ライフアップキノコ解説");
    }

    void onGameEventFlagOneUpAtFirst() {
        GameDataFunction::onGameEventFlag("１ＵＰキノコ解説");
    }

    bool isOnGameEventFlagPlayMoviePrologueA() {
        return GameDataFunction::isPassedStoryEvent("クッパ襲来後");
    }

    bool isOnGameEventFlagEndTicoGuideDemo() {
        return GameDataFunction::isPassedStoryEvent("チコガイドデモ終了");
    }

    bool isOnGameEventFlagEndButlerDomeLecture() {
        return GameDataFunction::isPassedStoryEvent("天球儀レクチャー");
    }

    bool isOnGameEventFlagEndButlerGalaxyMoveLecture() {
        return GameDataFunction::isPassedStoryEvent("ギャラクシー移動レクチャー");
    }

    bool isOnGameEventFlagEndButlerStarPieceLecture() {
        return GameDataFunction::isPassedStoryEvent("スターピースレクチャー");
    }

    bool isOnGameEventFlagBeeMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("ハチマリオ初変身");
    }

    bool isOnGameEventFlagTeresaMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("テレサマリオ初変身");
    }

    bool isOnGameEventFlagHopperMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("ホッパーマリオ初変身");
    }

    bool isOnGameEventFlagFireMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("ファイアマリオ初変身");
    }

    bool isOnGameEventFlagIceMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("アイスマリオ初変身");
    }

    bool isOnGameEventFlagFlyingMarioAtFirst() {
        return GameDataFunction::isOnGameEventFlag("フライングマリオ初変身");
    }

    bool isOnGameEventFlagSurfingTutorialAtFirst() {
        return GameDataFunction::isOnGameEventFlag("サーフィンチュートリアル");
    }

    bool isOnGameEventFlagTamakoroTutorialAtFirst() {
        return GameDataFunction::isOnGameEventFlag("タマコロチュートリアル");
    }

    bool isOnGameEventFlagViewCompleteEnding() {
        return GameDataFunction::isOnGameEventFlag("ViewCompleteEnding");
    }

    bool isOnGameEventFlagViewCompleteEndingMarioAndLuigi() {
        return GameDataFunction::isOnCompleteEndingMarioAndLuigi();
    }

    bool isOnGameEventFlagOffAstroDomeGuidance() {
        return GameDataFunction::hasGrandStar(2);
    }

    bool isOnGameEventFlagChildsRoom() {
        return MR::calcOpenedAstroDomeNum() >= 6;
    }

    bool isOnGameEventFlagLibraryRoom() {
        return GameDataFunction::getPictureBookChapterCanRead() > 0;
    }

    bool isOnGameEventFlagRosettaTalkAboutTicoInTower() {
        return GameDataFunction::isOnGameEventFlag("RosettaTalkAboutTico");
    }

    bool isRosettaTalkTorchProgress() {
        return GameDataFunction::hasGrandStar(6);
    }

    bool isRosettaTalkCountDownStart() {
        return GameDataFunction::hasGrandStar(5);
    }

    bool isRosettaTalkAstroDomeRecover() {
        return GameDataFunction::hasGrandStar(4);
    }

    bool isRosettaTalkKoopa() {
        return GameDataFunction::hasGrandStar(3);
    }

    bool isRosettaTalkTorchLecture() {
        return GameDataFunction::hasGrandStar(2);
    }

    bool isKinopioExplorerCompleteTrickComet() {
        return false;
    }

    bool isKinopioExplorerTalkGoFinalBattle() {
        return GameDataFunction::canOnGameEventFlag("KoopaBattleVs3Galaxy");
    }

    bool isKinopioExplorerTalkGetGrandStar6() {
        return GameDataFunction::hasGrandStar(6);
    }

    bool isKinopioExplorerTalkGetGrandStar5() {
        return GameDataFunction::hasGrandStar(5);
    }

    bool isKinopioExplorerTalkGetGrandStar4() {
        return GameDataFunction::hasGrandStar(4);
    }

    bool isKinopioExplorerTalkGetGrandStar3() {
        return GameDataFunction::hasGrandStar(3);
    }

    bool isKinopioExplorerTrickComet() {
        return GameDataFunction::isOnGameEventFlag("EventCometStarter");
    }

    bool isKinopioExplorerTalkGetGrandStar2() {
        return GameDataFunction::hasGrandStar(2);
    }

    bool isUFOKinokoBeforeConstruction() {
        return !GameDataFunction::isOnGameEventFlag("EventKinopioExplorerRescued");
    }

    bool isUFOKinokoUnderConstruction() {
        return !GameDataFunction::isOnGameEventFlag("EventKinopioExplorerOrganize");
    }
    
    bool isButlerMapAppear() {
        return GameDataFunction::hasGrandStar(2);
    }

    s32 setupAlreadyDoneFlag(const char* pParam1, const JMapInfoIter& rIter, u32* pParam3) {
        return GameDataFunction::setupAlreadyDoneFlag(pParam1, rIter, pParam3);
    }

    void updateAlreadyDoneFlag(int param1, u32 param2) {
        GameDataFunction::updateAlreadyDoneFlag(param1, param2);
    }

    bool isOnGameEventFlagGalaxyOpen(const char* pKey) {
        return GameDataFunction::isOnGameEventFlag(pKey);
    }

    bool isAlreadyVisitedCurrentStageAndScenario() {
        return GameDataFunction::isOnGalaxyScenarioFlagAlreadyVisited(
            MR::getCurrentStageName(),
            MR::getCurrentScenarioNo());
    }

    bool isAlreadyVisitedStage(const char* pGalaxyName) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);

        for (s32 i = 1; i <= accessor.getScenarioNum(); i++) {
            if (GameDataFunction::isOnGalaxyScenarioFlagAlreadyVisited(pGalaxyName, i)) {
                return true;
            }
        }

        return false;
    }

    bool canOpenGalaxy(const char* pGalaxyName) {
        return GameDataFunction::canOnGameEventFlag(pGalaxyName);
    }

    bool isAppearGalaxy(const char* pGalaxyName) {
        return GameDataFunction::isAppearGalaxy(pGalaxyName);
    }

    bool isGalaxyAppearGreenDriver(const char* pGalaxyName) {
        return GameDataConst::isGalaxyAppearGreenDriver(pGalaxyName);
    }

    void onGameEventFlagGalaxyOpen(const char* pKey) {
        GameDataFunction::onGameEventFlag(pKey);
    }

    bool hasPowerStarInCurrentStage(s32 starId) {
        return MR::makeCurrentGalaxyStatusAccessor().hasPowerStar(starId);
    }

    bool isPowerStarGreenInCurrentStage(s32 starId) {
        return GameDataConst::isPowerStarGreen(MR::getCurrentStageName(), starId);
    }

    bool isPowerStarRedInCurrentStage(s32 starId) {
        return GameDataConst::isPowerStarRed(MR::getCurrentStageName(), starId);
    }

    bool isGrandStarInCurrentStage(s32 starId) {
        return GameDataConst::isGrandStar(MR::getCurrentStageName(), starId);
    }

    bool hasPowerStarInCurrentStageWithDeclarer(const char* pParam1, s32 param2) {
        return MR::isSuccessEventPowerStar(pParam1, param2);
    }

    bool isPowerStarGreenInCurrentStageWithDeclarer(const char* pParam1, s32 param2) {
        return MR::isGreenEventPowerStar(pParam1, param2);
    }

    bool isPowerStarRedInCurrentStageWithDeclarer(const char* pParam1, s32 param2) {
        return MR::isRedEventPowerStar(pParam1, param2);
    }

    bool isGrandStarInCurrentStageWithDeclarer(const char* pParam1, s32 param2) {
        return MR::isGrandEventPowerStar(pParam1, param2);
    }

    bool hasPowerStarAtResultSequence() {
        return GameSequenceFunction::hasPowerStarYetAtResultSequence();
    }

    bool isPowerStarGreenAtResultSequence() {
        return GameSequenceFunction::isPowerStarGreenAtResultSequence();
    }

    bool isPowerStarRedAtResultSequence() {
        return GameSequenceFunction::isPowerStarRedAtResultSequence();
    }

    void sendStageResultSequenceParam(s32 scenarioNo) {
        GameSequenceFunction::sendStageResultSequenceParam(
            MR::getCurrentStageName(),
            scenarioNo,
            MR::getStarPieceNum(),
            MR::getCoinNum());
    }

    bool isOnGameEventFlagPowerStarSuccess(const char* pGalaxyName, s32 starId) {
        return GameDataFunction::hasPowerStar(pGalaxyName, starId);
    }

    bool hasGrandStar(int grandStarId) {
        return GameDataFunction::hasGrandStar(grandStarId);
    }

    s32 calcOpenedAstroDomeNum() {
        s32 openedAstroDomeNum;

        if (GameSequenceFunction::hasStageResultSequence()) {
            openedAstroDomeNum = GameDataFunction::calcGrandStarNum(GameDataFunction::getSceneStartGameDataHolder());
        }
        else {
            openedAstroDomeNum = GameDataFunction::calcGrandStarNum(GameDataFunction::getCurrentGameDataHolder());
        }            

        if (GameDataFunction::hasGrandStar(7)) {
            openedAstroDomeNum -= 1;
        }

        return openedAstroDomeNum;
    }

    s32 calcCurrentGreenStarNum() {
        return GameDataFunction::calcGreenStarNum(GameDataFunction::getCurrentGameDataHolder());
    }

    s32 getPowerStarNumToOpenGalaxy(const char* pGalaxyName) {
        return GameDataConst::getPowerStarNumToOpenGalaxy(pGalaxyName);
    }

    s32 getPowerStarNumSucceed(const char* pGalaxyName) {
        return MR::makeGalaxyStatusAccessor(pGalaxyName).getPowerStarNumOwned();
    }

    bool isPowerStarGreen(const char* pGalaxyName, s32 starId) {
        return GameDataConst::isPowerStarGreen(pGalaxyName, starId);
    }

    s32 getPictureBookChapterCanRead() {
        return GameDataFunction::getPictureBookChapterCanRead();
    }

    s32 getPictureBookChapterAlreadyRead() {
        return GameDataFunction::getPictureBookChapterAlreadyRead();
    }

    void setPictureBookChapterAlreadyRead(int chapterAlreadyRead) {
        GameDataFunction::setPictureBookChapterAlreadyRead(chapterAlreadyRead);
    }

    void setRaceBestTime(int id, u32 bestTime) {
        const char* pRaceName = RaceManagerFunction::getRaceName(id);

        GameDataFunction::setRaceBestTime(pRaceName, bestTime);
    }

    u32 getRaceBestTime(int id) {
        const char* pRaceName = RaceManagerFunction::getRaceName(id);

        return GameDataFunction::getRaceBestTime(pRaceName);
    }

    u32 getRaceCurrentTime() {
        return RaceManagerFunction::getRaceTime();
    }

    void setWarpPodPathFlag(int bit, bool isOn) {
        GameDataFunction::setGameEventValueForBit("WarpPodSaveBits", bit, isOn);
    }

    s32 registerStorageSpinDriverPathDrawRange(const NameObj* pParam1, const JMapInfoIter& rIter, int param3, f32* pParam4) {
        const char* pStageName = MR::getCurrentStageName();
        int scenarioNo = MR::getCurrentScenarioNo();
        int zoneId = MR::getPlacedZoneId(rIter);

        if (param3 <= 0) {
            return -1;
        }

        return GameDataFunction::registerStorageSpinDriverPathDrawRange(pStageName, scenarioNo, zoneId, param3, pParam4);
    }

    void updateStorageSpinDriverPathDrawRange(int param1, f32 param2) {
        const char* pStageName = MR::getCurrentStageName();
        s32 scenarioNo = MR::getCurrentScenarioNo();

        GameDataFunction::updateStorageSpinDriverPathDrawRange(pStageName, scenarioNo, param1, param2);
    }

    s32 getStarPieceNumGivingToTicoSeed(int index) {
        return GameDataFunction::getStarPieceNumGivingToTicoSeed(index);
    }

    void addStarPieceGivingToTicoSeed(int index, int num) {
        GameDataFunction::addStarPieceGivingToTicoSeed(index, num);
    }

    s32 getStarPieceNumGivingToTicoGalaxy(int index) {
        return GameDataFunction::getStarPieceNumGivingToTicoSeed(index + 8);
    }

    s32 getStarPieceNumMaxGivingToTicoGalaxy(int index) {
        return GameDataFunction::getStarPieceNumMaxGivingToTicoSeed(index + 8);
    }

    void addStarPieceGivingToTicoGalaxy(int index, int num) {
        GameDataFunction::addStarPieceGivingToTicoSeed(index + 8, num);
    }

    void setTicoGalaxyAlreadyTalk(int bit, bool isTalk) {
        GameDataFunction::setGameEventValueForBit("TicoGalaxyAlreadyTalk", bit, isTalk);
    }

    // isGalaxyAnyCometAppearInCurrentStage

    void startGalaxyCometEvent() {
        EventFunction::startCometEvent();
    }

    void endGalaxyCometEvent() {
        EventFunction::endCometEvent();
    }

    s32 getGalaxyCometStateIndexInCurrentStage() {
        return GameSequenceFunction::getGalaxyCometState(MR::getCurrentStageName());
    }

    void forceToNextStateGalaxyCometScheduler() {
        GameSequenceFunction::forceToNextStateGalaxyCometScheduler();
    }

    bool isGalaxyCometLandInStage(const char* pParam1) {
        return GameSequenceFunction::isGalaxyCometLand(pParam1);
    }

    s32 getEncounterGalaxyCometPowerStarId(const char* pParam1) {
        return GameSequenceFunction::getEncounterGalaxyCometPowerStarId(pParam1);
    }

    bool isGalaxyPurpleCometLaunch() {
        return MR::isOnGameEventFlagGalaxyOpen("ViewNormalEnding");
    }

    bool canAppearNormalComet(const char* pGalaxyName) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        bool isOnComet = GameDataFunction::isOnGameEventFlagNormalComet(pGalaxyName);
        bool hasPowerStar = GameDataFunction::hasPowerStar(pGalaxyName, accessor.getNormalCometScenarioNo());

        return isOnComet && !hasPowerStar;
    }

    bool canAppearCoin100Comet(const char* pGalaxyName) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        bool isOnComet = GameDataFunction::isOnGameEventFlagCoin100Comet(pGalaxyName);
        bool hasPowerStar = GameDataFunction::hasPowerStar(pGalaxyName, accessor.getCoin100CometScenarioNo());

        return isOnComet && !hasPowerStar;
    }

    int getEncounterGalaxyCometNameId(const char* pGalaxyName) {
        return MR::getCometNameIdFromString(GameSequenceFunction::getEncounterGalaxyCometName(pGalaxyName));
    }

    int getCometNameIdFromString(const char* pCometName) {
        if (MR::isEqualString(pCometName, "Red")) {
            return 0;
        }

        if (MR::isEqualString(pCometName, "Dark")) {
            return 2;
        }

        if (MR::isEqualString(pCometName, "Ghost")) {
            return 1;
        }

        if (MR::isEqualString(pCometName, "Quick")) {
            return 3;
        }

        if (MR::isEqualString(pCometName, "Purple")) {
            return 4;
        }

        if (MR::isEqualString(pCometName, "Black")) {
            return 4;
        }

        return 0;
    }

    const char* getCometNameFromId(int cometId) {
        switch (cometId) {
        case 0:
            return "Red";
        case 2:
            return "Dark";
        case 1:
            return "Ghost";
        case 3:
            return "Quick";
        case 4:
            return "Purple";
        default:
            return nullptr;
        }
    }

    bool isStarComplete(const char* pGalaxyName) {
        GalaxyStatusAccessor aAccessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        GalaxyStatusAccessor bAccessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        s32 powerStarNumOwned = bAccessor.getPowerStarNumOwned();
        s32 powerStarNum = aAccessor.getPowerStarNum();

        return powerStarNumOwned == powerStarNum;
    }

    bool isStarCompleteNormalScenario(const char* pGalaxyName) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);

        for (s32 i = 1; i <= accessor.getNormalScenarioNum(); i++) {
            if (!GameDataFunction::hasPowerStar(pGalaxyName, i)) {
                return false;
            }
        }

        return true;
    }

    /*
    // TODO: GameDataSomeScenarioAccessor has not yet been declared.
    s32 getCoinBestScore(const char* pGalaxyName, s32 scenarioNo) {
        GameDataSomeScenarioAccessor accessor = GameDataFunction::makeGalaxyScenarioAccessor(pGalaxyName, scenarioNo);

        return accessor.getMaxCoinNum();
    }
    */

    s32 getCoinBestScore(const char* pGalaxyName) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        s32 coinBestScore = 0;

        for (s32 i = 1; i <= accessor.getPowerStarNum(); i++) {
            if (!MR::isPlacedCoin(pGalaxyName, i)) {
                continue;
            }

            s32 coinBestScoreScenario = MR::getCoinBestScore(pGalaxyName, i);

            if (coinBestScore < coinBestScoreScenario) {
                coinBestScore = coinBestScoreScenario;
            }
        }

        return coinBestScore;
    }

    bool isPlacedCoin(const char* pGalaxyName, s32 scenarioNo) {
        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(pGalaxyName);
        const char* pCometName = accessor.getCometName(scenarioNo);

        return pCometName == nullptr || !MR::isEqualString(pCometName, "Dark");
    }

    bool isActiveLuigiHideAndSeekEvent() {
        return GameSequenceFunction::isActiveLuigiHideAndSeekEvent();
    }

    bool isEndLuigiHideAndSeekEvent() {
        return GameSequenceFunction::isEndLuigiHideAndSeekEvent();
    }

    bool isLuigiDisappearFromAstroGalaxy() {
        return GameSequenceFunction::isLuigiDisappearFromAstroGalaxy();
    }

    bool isOnGameEventFlagLuigiRescued() {
        return MR::isOnGameEventFlagGalaxyOpen("SpecialStarLuigiRescued");
    }

    void onGameEventFlagTalkedToLuigiAfterRescued() {
        MR::onGameEventFlagGalaxyOpen("LuigiTalkAfterRescued");
    }

    void onGameEventFlagGetLuigiLetter() {
        GameSequenceFunction::onGameEventFlagGetLuigiLetter();
    }

    bool isOnLuigiHiding() {
        s32 scenarioNo = MR::getCurrentSelectedScenarioNo();
        bool v1 = scenarioNo != -1
            && GameDataConst::isPowerStarLuigiHas(MR::getCurrentStageName(), scenarioNo);

        if (v1) {
            return true;
        }

        return GameSequenceFunction::isLuigiHidingAnyGalaxy();
    }

    bool isOnLuigiHidingCurrentStage() {
        if (!MR::isOnLuigiHiding()) {
            return false;
        }

        s32 scenarioNo = MR::getCurrentSelectedScenarioNo();
        bool v1 = scenarioNo != -1
            && GameDataConst::isPowerStarLuigiHas(MR::getCurrentStageName(), scenarioNo);

        if (v1) {
            return true;
        }

        s32 starId = 0;
        const char* pHidingGalaxyName = nullptr;

        GameSequenceFunction::getLuigiHidingGalaxyNameAndStarId(&pHidingGalaxyName, &starId);

        return MR::isEqualString(pHidingGalaxyName, MR::getCurrentStageName());
    }

    bool isLuigiDisappearFromAstroGalaxyOrHiding() {
        bool isLuigiDisappearFromAstroGalaxy = GameSequenceFunction::isLuigiDisappearFromAstroGalaxy();

        if (isLuigiDisappearFromAstroGalaxy) {
            return MR::isOnLuigiHiding();
        }
        else {
            return isLuigiDisappearFromAstroGalaxy;
        }
    }

    bool isLuigiLetterArrivalAtMessenger() {
        bool isLuigiDisappearFromAstroGalaxy = GameSequenceFunction::isLuigiDisappearFromAstroGalaxy();

        if (isLuigiDisappearFromAstroGalaxy) {
            return !MR::isOnLuigiHiding();
        }
        else {
            return isLuigiDisappearFromAstroGalaxy;
        }
    }

    // isLuigiHidingGalaxyAndScenario

    bool isPowerStarGetDemoWithLuigiCurrentGalaxy() {
        if (MR::isOnLuigiHidingCurrentStage()) {
            return true;
        }

        return GameDataConst::isGalaxyLuigiArrested(MR::getCurrentStageName(), -1);
    }

    // isPowerStarGetDemoWithLuigiCurrentGalaxyAndScenario
    // getLuigiLetterGalaxyName

    // FIXME: Improper stack accesses.
    const char* getLuigiLetterGalaxyNameForNPC() {
        s32 starId;
        const char* pGalaxyName;

        if (GameSequenceFunction::isLuigiDisappearFromAstroGalaxy() || MR::isOnLuigiHiding()) {
            pGalaxyName = nullptr;

            GameSequenceFunction::getLuigiHidingGalaxyNameAndStarId(&pGalaxyName, &starId);

            return pGalaxyName;
        }

        return nullptr;
    }

    void onMessageAlreadyRead(s8 bit) {
        u16 value = GameDataFunction::getGameEventValue("MessageAlreadyRead");
        s32 mask = 1 << bit;

        GameDataFunction::setGameEventValue("MessageAlreadyRead", value | mask);
    }

    void offMsgLedPattern() {
        GameDataFunction::setGameEventValue("MsgLedPattern", 0);
    }

    bool isMsgLedPattern() {
        return GameDataFunction::getGameEventValue("MsgLedPattern") != 0;
    }

    void explainBeeMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("ハチマリオ初変身")) {
            InformationObserverFunction::explainBee();
        }
    }

    void explainTeresaMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("テレサマリオ初変身")) {
            InformationObserverFunction::explainTeresa();
        }
    }

    void explainHopperMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("ホッパーマリオ初変身")) {
            InformationObserverFunction::explainHopper();
        }
    }

    void explainFireMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("ファイアマリオ初変身")) {
            InformationObserverFunction::explainFire();
        }
    }

    void explainIceMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("アイスマリオ初変身")) {
            InformationObserverFunction::explainIce();
        }
    }

    void explainFlyingMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("フライングマリオ初変身")) {
            InformationObserverFunction::explainFlying();
        }
    }

    void explainInvincibleMarioIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("無敵マリオ初変身")) {
            InformationObserverFunction::explainInvincible();
        }
    }

    void explainLifeUpIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("ライフアップキノコ解説")) {
            InformationObserverFunction::explainLifeUp();
        }
    }

    void explainOneUpIfAtFirst() {
        if (!GameDataFunction::isOnGameEventFlag("１ＵＰキノコ解説")) {
            InformationObserverFunction::explainOneUp();
        }
    }

    void onGameEventFlagPlayMoviePrologueA() {
        GameDataFunction::followStoryEventByName("クッパ襲来後");
    }

    void onGameEventFlagPlayMoviePrologueB() {
        GameDataFunction::followStoryEventByName("ピーチ城浮上後");
    }

    void onGameEventFlagEndTicoGuideDemo() {
        GameDataFunction::followStoryEventByName("チコガイドデモ終了");
    }

    void onGameEventFlagEnableToSpinAndStarPointer() {
        GameDataFunction::followStoryEventByName("スピン権利");
        MR::setPlayerSwingPermission(true);
    }

    void onGameEventFlagEndButlerDomeLecture() {
        GameDataFunction::followStoryEventByName("天球儀レクチャー");
    }

    void onGameEventFlagEndButlerGalaxyMoveLecture() {
        GameDataFunction::followStoryEventByName("ギャラクシー移動レクチャー");
    }

    void onGameEventFlagEndButlerStarPieceLecture() {
        GameDataFunction::followStoryEventByName("スターピースレクチャー");
    }

    void onGameEventFlagSurfingTutorialAtFirst() {
        GameDataFunction::onGameEventFlag("サーフィンチュートリアル");
    }

    void onGameEventFlagTamakoroTutorialAtFirst() {
        GameDataFunction::onGameEventFlag("タマコロチュートリアル");
    }

    void onGameEventFlagRosettaTalkAboutTicoInTower() {
        GameDataFunction::onGameEventFlag("RosettaTalkAboutTico");
    }

    void onGameEventFlagViewNormalEnding() {
        GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor("SpecialStarGrand7");

        GameDataFunction::setGameFlagPowerStarSuccess(accessor.getGalaxyName(), accessor.getStarId(), true);
    }

    void onGameEventFlagViewCompleteEnding() {
        GameDataFunction::onGameEventFlag("ViewCompleteEnding");
        GameDataFunction::onCompleteEndingCurrentPlayer();
    }

    bool isOnGameEventFlagUseAstroDome() {
        return GameDataFunction::isPassedStoryEvent("バトラー情報Ａ");
    }

    bool isOnGameEventFlagGreenDriver() {
        return GameDataFunction::isOnGameEventFlag("SpecialStarGreenAll");
    }

    bool isOnGameEventFlagRedDriver() {
        return GameDataFunction::isOnGameEventFlag("SpecialStarRed1");
    }

    bool isOnGameEventFlagViewNormalEnding() {
        return GameDataFunction::isOnGameEventFlag("ViewNormalEnding");
    }

    bool isRosettaTalkTrickComet() {
        return GameDataFunction::isOnGameEventFlag("EventCometStarter");
    }

    bool isKinopioExplorerStartMessenger() {
        return GameDataFunction::isOnGameEventFlag("SpecialStarLuigiRescued");
    }

    bool isKinopioExplorerOrganize() {
        return GameDataFunction::isOnGameEventFlag("EventKinopioExplorerOrganize");
    }

    bool isKinopioExplorerRescued() {
        return GameDataFunction::isOnGameEventFlag("EventKinopioExplorerRescued");
    }

    s32 getPowerStarLeftToDisplayCountDownPlate() {
        s32 powerStarNumTarget = MR::isOnGameEventFlagGalaxyOpen("ViewNormalEnding")
            ? GameDataFunction::getPowerStarNumMax() - 1
            : GameDataConst::getPowerStarNumToOpenGalaxy("KoopaBattleVs3Galaxy");
        s32 powerStarLeft = powerStarNumTarget - GameDataFunction::calcCurrentPowerStarNum();

        if (powerStarLeft < 0) {
            powerStarLeft = 0;
        }
        else if (powerStarLeft > powerStarNumTarget) {
            powerStarLeft = powerStarNumTarget;
        }

        if (GameDataFunction::isEqualJustPowerStarNum(powerStarNumTarget)
            || GameDataFunction::isOnJustGameEventFlag("SpecialStarGrand5")
            && powerStarLeft == 0)
        {
            powerStarLeft += 1;
        }

        return powerStarLeft;
    }

    bool isOnWarpPodPathFlag(int bit) {
        return GameDataFunction::isOnGameEventValueForBit("WarpPodSaveBits", bit);
    }

    bool isOnTicoGalaxyAlreadyTalk(int bit) {
        return GameDataFunction::isOnGameEventValueForBit("TicoGalaxyAlreadyTalk", bit);
    }

    bool isKoopaFortressAppearInGalaxy() {
        char key[32];

        if (!MR::isEqualStageName("AstroDome")) {
            return false;
        }

        s32 scenarioNo = MR::getCurrentScenarioNo();

        if (scenarioNo == 6) {
            return false;
        }

        snprintf(key, sizeof(key), "SpecialStarGrand%1d", scenarioNo + 1);

        GameEventFlagAccessor accessor = GameEventFlagTable::makeAccessor(key);

        return MR::isOnGameEventFlagGalaxyOpen(accessor.getGalaxyName())
            && !MR::isOnGameEventFlagGalaxyOpen(key);
    }

    bool isGalaxyRedCometAppearInCurrentStage() {
        return EventFunction::isStartCometEvent("Red");
    }

    bool isGalaxyDarkCometAppearInCurrentStage() {
        return EventFunction::isStartCometEvent("Dark");
    }

    bool isGalaxyGhostCometAppearInCurrentStage() {
        return EventFunction::isStartCometEvent("Ghost");
    }

    bool isGalaxyQuickCometAppearInCurrentStage() {
        return EventFunction::isStartCometEvent("Quick");
    }

    bool isGalaxyBlackCometAppearInCurrentStage() {
        return EventFunction::isStartCometEvent("Black");
    }

    bool isStarCompleteAllGalaxy() {
        return GameDataFunction::isOnGameEventFlag("PowerStarComplete");
    }

    bool isStarPieceCounterStop() {
        return GameDataFunction::isOnGameEventFlag("StarPieceCounterStop");
    }

    bool isOnGameEventFlagAstroGalaxyBgmBright() {
        return GameDataFunction::hasPowerStar("EggStarGalaxy", 1);
    }

    bool isOnMessageAlreadyRead(s8 bit) {
        s32 value = GameDataFunction::getGameEventValue("MessageAlreadyRead");
        s32 mask = 1 << bit;

        return (mask & value) != 0;
    }

    void onMsgLedPattern() {
        GameDataFunction::setGameEventValue("MsgLedPattern", 1);
    }
};

#include "Game/Util/EventUtil.hpp"
#include "Game/NPC/EventDirector.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Scene/ScenePlayingResult.hpp"
#include "Game/Screen/InformationObserver.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataConst.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/Util/SceneUtil.hpp"

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

    bool isAnyPlayerLeftSupply() {
        bool ret = true;
        bool cond1 = true;
        if (GameDataFunction::isPlayerLeftSupply() == false && GameDataFunction::isPointCollectForLetter() == false) {
            cond1 = false;
        }
        if (cond1 == false && GameDataFunction::isLuigiLeftSupply() == false) {
            ret = false;
        }
        return ret;
    }

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
        if (isStageAstroLocation()) {
            return GameDataFunction::getStockedStarPieceNum();
        }
        else {
            return getSceneObj<ScenePlayingResult*>(SceneObj_ScenePlayingResult)->getStarPieceNum();
        }
    }

    void addStarPiece(int n) {
        if (isStageAstroLocation()) {
            GameDataFunction::addStockedStarPiece(n);
        }
        else {
            getSceneObj<ScenePlayingResult*>(SceneObj_ScenePlayingResult)->incStarPiece(n);
        }
    }

    s32 getStockedStarPieceNum() {
        return GameDataFunction::getStockedStarPieceNum();
    }

    void addStockedStarPiece(int n) {
        return GameDataFunction::addStockedStarPiece(n);
    }

    bool isPlayerLuigi() {
        return !GameDataFunction::isDataMario();
    }

    void explainEnableToSpin(LiveActor *pActor) {
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

    s32 setupAlreadyDoneFlag(const char *a1, const JMapInfoIter &a2, u32 *a3) {
        return GameDataFunction::setupAlreadyDoneFlag(a1, a2, a3);
    }

    void updateAlreadyDoneFlag(int a1, u32 a2) {
        GameDataFunction::updateAlreadyDoneFlag(a1, a2);
    }

    bool isOnGameEventFlagGalaxyOpen(const char *pName) {
        return GameDataFunction::isOnGameEventFlag(pName);
    }

    bool isAlreadyVisitedCurrentStageAndScenario() {
        s32 scenarioNo = getCurrentScenarioNo();
        return GameDataFunction::isOnGalaxyScenarioFlagAlreadyVisited(getCurrentStageName(), scenarioNo);
    }

    //Compared with cmplw instead of cmpw
    bool isAlreadyVisitedStage(const char *pStageName) {
        GalaxyStatusAccessor pAccessor = makeGalaxyStatusAcessor(pStageName);
        s32 i = 1;
        while (pAccessor.getScenarioNum() < i == false) {
            if (GameDataFunction::isOnGalaxyScenarioFlagAlreadyVisited(pStageName, i)) {
                return true;
            }
            i++;
        }
        return false;
    }

    bool canOpenGalaxy(const char *pGalaxyName) {
        return GameDataFunction::canOnGameEventFlag(pGalaxyName);
    }

    bool isAppearGalaxy(const char *pGalaxyName) {
        return GameDataFunction::isAppearGalaxy(pGalaxyName);
    }

    bool isAppearGalaxyGreenDriver(const char *pGalaxyName) {
        return GameDataFunction::isAppearGalaxyGreenDriver(pGalaxyName);
    }

    void onGameEventFlagGalaxyOpen(const char *pGalaxyName) {
        return GameDataFunction::onGameEventFlag(pGalaxyName);
    }

    bool hasPowerStarInCurrentStage(s32 a1) {
        GalaxyStatusAccessor pAccessor = makeCurrentGalaxyStatusAccessor();
        return pAccessor.hasPowerStar(a1);
    }

    bool isPowerStarGreenInCurrentStage(s32 a1) {
        return GameDataConst::isPowerStarGreen(getCurrentStageName(), a1);
    }

    bool isPowerStarRedInCurrentStage(s32 a1) {
        return GameDataConst::isPowerStarRed(getCurrentStageName(), a1);
    }

    bool isGrandStarInCurrentStage(s32 a1) {
        return GameDataConst::isGrandStar(getCurrentStageName(), a1);
    }

    bool hasPowerStarInCurrentStageWithDeclarer(const char *pStageName, s32 a2) {
        return isSuccessEventPowerStar(pStageName, a2);
    }

    bool isPowerStarGreenInCurrentStageWithDeclarer(const char *pStageName, s32 a2) {
        return isGreenEventPowerStar(pStageName, a2);
    }
    
    bool isPowerStarRedInCurrentStageWithDeclarer(const char *pStageName, s32 a2) {
        return isRedEventPowerStar(pStageName, a2);
    }

    bool isGrandStarInCurrentStageWithDeclarer(const char *pStageName, s32 a2) {
        return isGrandEventPowerStar(pStageName, a2);
    }

    bool hasPowerStarAtResultSequence() {
        return GameDataFunction::hasPowerStarYetAtResultSequence();
    }

    bool isPowerStarGreenAtResultSequence() {
        return GameDataFunction::isPowerStarGreenAtResultSequence();
    }

    bool isPowerStarRedAtResultSequence() {
        return GameDataFunction::isPowerStarRedAtResultSequence();
    }

    void sendStageResultSequenceParam(s32 a1) {
        GameSequenceFunction::sendStageResultSequenceParam(getCurrentStageName(), a1, getStarPieceNum(), getCoinNum());
    }

    bool isOnGameEventFlagPowerStarSuccess(const char *a1, s32 a2) {
        return GameDataFunction::hasPowerStar(a1, a2);
    }

    bool hasGrandStar(int n) {
        return GameDataFunction::hasGrandStar(n);
    }
};

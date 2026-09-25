#include "Game/Screen/StageResultInformer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/GalaxyMapController.hpp"
#include "Game/Screen/IconAButton.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameDataGalaxyStorage.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/StageResultSequenceChecker.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MessageUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    const char cMessageIdGetNormalStar[] = "System_Result000";
    const char cMessageIdGetHiddenStar[] = "System_Result003";
    const char cMessageIdGalaxyComplete[] = "System_Result009";
    const char cMessageIdGetStarPiece[] = "System_Result004";
    const char cMessageIdNewGalaxyDiscover[] = "System_Result001";
    const char cMessageIdOpenLibraryRoom[] = "System_Result008";
    const char cMessageIdGetPictureBook[] = "System_Result005";
    const char cMessageIdUpdateBestScore[] = "System_Result006";
    const char cMessageIdTicoGalaxyAppear[] = "System_Result007";
    const char cNamePowerStarNum[] = "ShaStarNum";
    const char cNameGalaxyStarPieceNum[] = "ShaTPieceLNum";
    const char cNameTotalStarPieceNum[] = "ShaTPieceRNum";
    const char cNameBestCoinNum[] = "ShaCoinNum";
    const char cNameBestCoinRoot[] = "Coin";
    static const s32 sWaitBeforeCountUpPowerStar = 85;
    static const s32 sWaitAppearInformationWindow = 15;
    static const s32 sWaitDisplayStarPieceCount = 45;
}  // namespace

namespace {
    NEW_NERVE(StageResultInformerAppearGetPowerStar, StageResultInformer, AppearGetPowerStar);
    NEW_NERVE(StageResultInformerWaitBeforeCountUpPowerStar, StageResultInformer, WaitBeforeCountUpPowerStar);
    NEW_NERVE(StageResultInformerCountUpPowerStar, StageResultInformer, CountUpPowerStar);
    NEW_NERVE(StageResultInformerDisplayGetPowerStar, StageResultInformer, DisplayGetPowerStar);
    NEW_NERVE(StageResultInformerDisappearGetPowerStar, StageResultInformer, DisappearGetPowerStar);
    NEW_NERVE(StageResultInformerShowGalaxyComplete, StageResultInformer, ShowGalaxyComplete);
    NEW_NERVE(StageResultInformerAppearUpdateBestScore, StageResultInformer, AppearUpdateBestScore);
    NEW_NERVE(StageResultInformerDisplayUpdateBestScore, StageResultInformer, DisplayUpdateBestScore);
    NEW_NERVE(StageResultInformerDisappearUpdateBestScore, StageResultInformer, DisappearUpdateBestScore);
    NEW_NERVE(StageResultInformerAppearGetStarPiece, StageResultInformer, AppearGetStarPiece);
    NEW_NERVE(StageResultInformerDisplayGetStarPiece, StageResultInformer, DisplayGetStarPiece);
    NEW_NERVE(StageResultInformerWaitBeforeDisappearGetStarPiece, StageResultInformer, WaitBeforeDisappearGetStarPiece);
    NEW_NERVE(StageResultInformerDisappearGetStarPiece, StageResultInformer, DisappearGetStarPiece);
    NEW_NERVE(StageResultInformerShowNewGalaxyDiscover, StageResultInformer, ShowNewGalaxyDiscover);
    NEW_NERVE(StageResultInformerShowGalaxyMap, StageResultInformer, ShowGalaxyMap);
    NEW_NERVE(StageResultInformerShowGetPictureBook, StageResultInformer, ShowGetPictureBook);
    NEW_NERVE(StageResultInformerShowTicoGalaxyAppear, StageResultInformer, ShowTicoGalaxyAppear);
    NEW_NERVE(StageResultInformerShowAstroMapForTico, StageResultInformer, ShowAstroMapForTico);
    NEW_NERVE(DisplayInformationForResultAppear, DisplayInformationForResult, Appear);
    NEW_NERVE(DisplayInformationForResultDisplay, DisplayInformationForResult, Display);
    NEW_NERVE(DisplayInformationForResultDisappear, DisplayInformationForResult, Disappear);
    NEW_NERVE(DisplayInformationForResultIdle, DisplayInformationForResult, Idle);
}  // namespace

DisplayInformationForResult::DisplayInformationForResult(StageResultInformer* pHost) : NerveExecutor("DisplayInformationForResult"), mHost(pHost) {
    initNerve(GET_NERVE_ANON(DisplayInformationForResultIdle));
}

void DisplayInformationForResult::exeAppear() {
    if (MR::isFirstStep(this)) {
        if (MR::isEqualString(mMessageId, ::cMessageIdGalaxyComplete)) {
            MR::startSubBGM("BGM_GALAXY_COMPLETE", false);
        } else {
            MR::startSubBGM("BGM_NEW_GALAXY", false);
        }

        MR::appearInformationMessageCenter(mMessageId, true);
    }

    MR::setNerveAtStep(this, GET_NERVE_ANON(DisplayInformationForResultDisplay), ::sWaitAppearInformationWindow);
}

void DisplayInformationForResult::exeDisplay() {
    if (MR::testSystemPadTriggerDecide()) {
        MR::startSystemSE("SE_SY_TALK_OK");
        MR::startCSSound("CS_CLICK_CLOSE", nullptr, 0);
        setNerve(GET_NERVE_ANON(DisplayInformationForResultDisappear));
    }
}

void DisplayInformationForResult::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::disappearInformationMessage();
    }

    if (MR::isDeadInformationMessage()) {
        setNerve(GET_NERVE_ANON(DisplayInformationForResultIdle));
    }
}

void DisplayInformationForResult::exeIdle() {
}

StageResultInformer::StageResultInformer()
    : LayoutActor("リザルト通知", true), mInformationDisplayer(), mSequenceChecker(), mIconAButton(), _30(), _31() {
    mInformationDisplayer = new DisplayInformationForResult(this);
    mSequenceChecker = new StageResultSequenceChecker();
}

void StageResultInformer::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initLayoutManager("ResultWindow", 2);
    mIconAButton = MR::createAndSetupIconAButton(this, true, false);
    initNerve(GET_NERVE_ANON(StageResultInformerAppearGetPowerStar));
    initEffectKeeper(0, nullptr, nullptr);
}

void StageResultInformer::appear() {
    LayoutActor::appear();
    mIconAButton->kill();
    MR::requestMovementOn(mIconAButton);
    MR::hideLayout(this);
    initBestScoreWindow();
    setNerve(GET_NERVE_ANON(StageResultInformerAppearGetPowerStar));
}

void StageResultInformer::kill() {
    GameSequenceFunction::resetStageResultSequenceParam();
    LayoutActor::kill();
    mIconAButton->kill();
}

void StageResultInformer::exeAppearGetPowerStar() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        mSequenceChecker->check();
        mSequenceChecker->checkIfComplete(GameSequenceFunction::getClearedStageName());
        setCounterPaneShowHide(true, false, false);
        MR::startAnim(this, "Appear", 0);

        GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(GameSequenceFunction::getClearedStageName());

        bool isHiddenStar = accessor.isHiddenStar(GameSequenceFunction::getClearedPowerStarId());
        const char* pMessageId = isHiddenStar ? ::cMessageIdGetHiddenStar : ::cMessageIdGetNormalStar;

        MR::setTextBoxGameMessageRecursive(this, "Result", pMessageId);
        MR::setTextBoxArgStringRecursive(this, "Result", MR::getGalaxyNameOnCurrentLanguage(GameSequenceFunction::getClearedStageName()), 0);
        MR::setTextBoxArgStringRecursive(
            this, "Result",
            MR::getScenarioNameOnCurrentLanguage(GameSequenceFunction::getClearedStageName(), GameSequenceFunction::getClearedPowerStarId()), 1);
        MR::startAnim(this, "Line", 1);
        MR::setAnimFrameAndStopAdjustTextHeight(this, "Result", 1);
        MR::setTextBoxNumberRecursive(this, ::cNamePowerStarNum, mSequenceChecker->getPrevPowerStarNum());
        MR::startSubBGM("BGM_ASTRO_RETURN", false);
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_ANON(StageResultInformerWaitBeforeCountUpPowerStar), 0);
}

void StageResultInformer::exeWaitBeforeCountUpPowerStar() {
    StageResultSequenceChecker* pChecker = mSequenceChecker;
    bool hasNewStar = pChecker->getPrevPowerStarNum() != pChecker->getAfterPowerStarNum();
    tryWaitIntervalBeforeKeyWait(hasNewStar ? static_cast< const Nerve* >(GET_NERVE_ANON(StageResultInformerCountUpPowerStar)) :
                                              GET_NERVE_ANON(StageResultInformerDisplayGetPowerStar),
                                 ::sWaitBeforeCountUpPowerStar);
}

void StageResultInformer::exeCountUpPowerStar() {
    if (MR::isFirstStep(this)) {
        MR::setTextBoxNumberRecursive(this, ::cNamePowerStarNum, mSequenceChecker->getAfterPowerStarNum());
        MR::startAnim(this, "Flash", 0);
        MR::startSystemSE("SE_SY_RESULT_POW_STAR_INC");
        MR::emitEffect(this, "ResultStarCounter");
    }

    if (MR::testSystemPadTriggerDecide()) {
        _30 = true;
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_ANON(StageResultInformerDisplayGetPowerStar), 0);
}

void StageResultInformer::exeDisplayGetPowerStar() {
    if (MR::isFirstStep(this)) {
        mIconAButton->openWithoutMessage();
    }

    if (tryWaitSystemPadTriggerDecide(GET_NERVE_ANON(StageResultInformerDisappearGetPowerStar))) {
        return;
    }
}

void StageResultInformer::exeDisappearGetPowerStar() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mIconAButton->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        decideNextNerve();
    }
}

void StageResultInformer::exeShowGalaxyComplete() {
    tryShowAndKeyWaitInformationWindow(::cMessageIdGalaxyComplete, nullptr);
}

void StageResultInformer::exeAppearUpdateBestScore() {
    if (MR::isFirstStep(this)) {
        setCounterPaneShowHide(false, true, false);
        MR::startSubBGM("BGM_HIGH_SCORE", false);
        MR::startAnim(this, "Appear", 0);
        MR::setTextBoxGameMessageRecursive(this, "Result", ::cMessageIdUpdateBestScore);
        MR::startAnim(this, "Line", 1);
        MR::setAnimFrameAndStopAdjustTextHeight(this, "Result", 1);
        GameSequenceFunction::reflectStageResultSequenceCoin();
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_ANON(StageResultInformerDisplayUpdateBestScore), 0);
}

void StageResultInformer::exeDisplayUpdateBestScore() {
    if (MR::isFirstStep(this)) {
        mIconAButton->openWithoutMessage();
    }

    if (tryWaitSystemPadTriggerDecide(GET_NERVE_ANON(StageResultInformerDisappearUpdateBestScore))) {
        return;
    }
}

void StageResultInformer::exeDisappearUpdateBestScore() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
        mIconAButton->term();
    }

    if (MR::isAnimStopped(this, 0)) {
        decideNextNerve();
    }
}

void StageResultInformer::exeAppearGetStarPiece() {
    if (MR::isFirstStep(this)) {
        setCounterPaneShowHide(false, false, true);
        MR::startAnim(this, "Appear", 0);
        MR::setTextBoxGameMessageRecursive(this, "Result", ::cMessageIdGetStarPiece);
        MR::startAnim(this, "Line", 1);
        MR::setAnimFrameAndStopAdjustTextHeight(this, "Result", 1);
        mClearedStarPieceNum = GameSequenceFunction::getClearedStarPieceNum();
        MR::setTextBoxNumberRecursive(this, ::cNameGalaxyStarPieceNum, mClearedStarPieceNum);
        MR::setTextBoxNumberRecursive(this, ::cNameTotalStarPieceNum, MR::getStockedStarPieceNum());
    }

    MR::setNerveAtAnimStopped(this, GET_NERVE_ANON(StageResultInformerDisplayGetStarPiece), 0);
}

void StageResultInformer::exeDisplayGetStarPiece() {
    if (MR::isFirstStep(this)) {
        mIconAButton->openWithoutMessage();
        MR::emitEffect(this, "ResultPieceCounter");
    }

    bool hasStarPiece = false;

    if (mClearedStarPieceNum > 0) {
        mClearedStarPieceNum--;

        MR::addStockedStarPiece(1);

        if (mClearedStarPieceNum > 0) {
            if (mClearedStarPieceNum % 2 == 0) {
                MR::startSystemSE("SE_SY_STAR_PIECE_SUM_UP");
            }
        } else {
            MR::startSystemSE("SE_SY_STAR_PIECE_SUM_UP_END");
        }
    }

    if (mClearedStarPieceNum == 0) {
        hasStarPiece = true;
        MR::deleteEffect(this, "ResultPieceCounter");
    }

    MR::setTextBoxNumberRecursive(this, ::cNameGalaxyStarPieceNum, mClearedStarPieceNum);
    MR::setTextBoxNumberRecursive(this, ::cNameTotalStarPieceNum, MR::getStockedStarPieceNum());

    if (MR::isAnimStopped(this, 0) && !hasStarPiece) {
        MR::startAnim(this, "Flash", 0);
    }

    if (MR::testSystemPadTriggerDecide()) {
        MR::startSystemSE("SE_SY_TALK_OK");

        if (hasStarPiece) {
            setNerve(GET_NERVE_ANON(StageResultInformerDisappearGetStarPiece));
        } else {
            setNerve(GET_NERVE_ANON(StageResultInformerWaitBeforeDisappearGetStarPiece));
        }
    }
}

void StageResultInformer::exeWaitBeforeDisappearGetStarPiece() {
    if (MR::isFirstStep(this)) {
        mIconAButton->term();
        MR::addStockedStarPiece(mClearedStarPieceNum);
        MR::startSystemSE("SE_SY_STAR_PIECE_SUM_UP_END");
        mClearedStarPieceNum = 0;
        MR::setTextBoxNumberRecursive(this, ::cNameGalaxyStarPieceNum, 0);
        MR::setTextBoxNumberRecursive(this, ::cNameTotalStarPieceNum, MR::getStockedStarPieceNum());
        MR::deleteEffect(this, "ResultPieceCounter");
    }

    MR::setNerveAtStep(this, GET_NERVE_ANON(StageResultInformerDisappearGetStarPiece), ::sWaitDisplayStarPieceCount);
}

void StageResultInformer::exeDisappearGetStarPiece() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);

        if (mIconAButton->isOpen()) {
            mIconAButton->term();
        }

        MR::forceSyncStarPieceCounter();
    }

    if (MR::isAnimStopped(this, 0)) {
        decideNextNerve();
    }
}

void StageResultInformer::exeShowNewGalaxyDiscover() {
    tryShowAndKeyWaitInformationWindow(::cMessageIdNewGalaxyDiscover, GET_NERVE_ANON(StageResultInformerShowGalaxyMap));
}

void StageResultInformer::exeShowGalaxyMap() {
    if (MR::isFirstStep(this)) {
        MR::startAstroMapLayoutForNewGalaxyDiscover();
    } else {
        decideNextNerve();
    }
}

void StageResultInformer::exeShowGetPictureBook() {
    if (MR::isFirstStep(this)) {
        const char* pMessageId = ::cMessageIdGetPictureBook;

        if (mSequenceChecker->isJustOpenLibraryRoom()) {
            pMessageId = ::cMessageIdOpenLibraryRoom;
        }

        DisplayInformationForResult* pDisplayer = mInformationDisplayer;
        pDisplayer->mMessageId = pMessageId;
        pDisplayer->setNerve(GET_NERVE_ANON(DisplayInformationForResultAppear));
    }

    mInformationDisplayer->updateNerve();

    bool isNotIdle = !mInformationDisplayer->isNerve(GET_NERVE_ANON(DisplayInformationForResultIdle));

    if (!isNotIdle) {
        decideNextNerve();
    }
}

void StageResultInformer::exeShowTicoGalaxyAppear() {
    tryShowAndKeyWaitInformationWindow(::cMessageIdTicoGalaxyAppear, GET_NERVE_ANON(StageResultInformerShowAstroMapForTico));
}

void StageResultInformer::exeShowAstroMapForTico() {
    if (MR::isFirstStep(this)) {
        MR::startAstroMapLayoutForNewTicoGalaxyDiscover();
    } else {
        decideNextNerve();
    }
}

void StageResultInformer::initBestScoreWindow() {
    GameDataSomeScenarioAccessor accessor =
        GameDataFunction::makeGalaxyScenarioAccessor(GameSequenceFunction::getClearedStageName(), GameSequenceFunction::getClearedPowerStarId());
    s32 clearedCoinNum = GameSequenceFunction::getClearedCoinNum();

    MR::setTextBoxNumberRecursive(this, ::cNameBestCoinNum, clearedCoinNum);

    bool newMax = accessor.getMaxCoinNum() < clearedCoinNum;

    if (newMax) {
        MR::showPane(this, ::cNameBestCoinRoot);
    } else {
        MR::hidePane(this, ::cNameBestCoinRoot);
    }

    _31 = newMax;
}

void StageResultInformer::decideNextNerve() {
    bool afterStar = isNerve(GET_NERVE_ANON(StageResultInformerDisappearGetPowerStar));
    bool afterComplete = afterStar || isNerve(GET_NERVE_ANON(StageResultInformerShowGalaxyComplete));
    bool afterScore = afterComplete || isNerve(GET_NERVE_ANON(StageResultInformerDisappearUpdateBestScore));
    bool afterPiece = afterScore || isNerve(GET_NERVE_ANON(StageResultInformerDisappearGetStarPiece));
    bool afterMap = afterPiece || isNerve(GET_NERVE_ANON(StageResultInformerShowGalaxyMap));
    bool afterBook = afterMap || isNerve(GET_NERVE_ANON(StageResultInformerShowGetPictureBook));

    if (afterStar && mSequenceChecker->isJustCompleteGalaxy()) {
        setNerve(GET_NERVE_ANON(StageResultInformerShowGalaxyComplete));
    } else if (afterComplete && _31) {
        setNerve(GET_NERVE_ANON(StageResultInformerAppearUpdateBestScore));
    } else if (afterScore && GameSequenceFunction::isNeedToReflectStageResultSequenceStarPiece()) {
        setNerve(GET_NERVE_ANON(StageResultInformerAppearGetStarPiece));
    } else if (afterPiece && mSequenceChecker->isJustOpenGalaxyWithoutChallengeGalaxy()) {
        setNerve(GET_NERVE_ANON(StageResultInformerShowNewGalaxyDiscover));
    } else if (afterMap && mSequenceChecker->isAddPictureBook()) {
        setNerve(GET_NERVE_ANON(StageResultInformerShowGetPictureBook));
    } else if (afterBook && mSequenceChecker->isJustAppearTicoGalaxy()) {
        setNerve(GET_NERVE_ANON(StageResultInformerShowTicoGalaxyAppear));
    } else {
        kill();
    }
}

bool StageResultInformer::tryWaitSystemPadTriggerDecide(const Nerve* pNerve) {
    if (MR::testSystemPadTriggerDecide() || _30) {
        _30 = false;

        setNerve(pNerve);
        MR::startSystemSE("SE_SY_TALK_OK");

        return true;
    }

    return false;
}

bool StageResultInformer::tryWaitIntervalBeforeKeyWait(const Nerve* pNerve, int step) {
    if (MR::testSystemPadTriggerDecide()) {
        _30 = true;
    }

    if (MR::isStep(this, step)) {
        setNerve(pNerve);

        return true;
    }

    return false;
}

bool StageResultInformer::tryShowAndKeyWaitInformationWindow(const char* pMessageId, const Nerve* pNerve) {
    if (MR::isFirstStep(this)) {
        DisplayInformationForResult* pInformationDisplayer = mInformationDisplayer;
        pInformationDisplayer->mMessageId = pMessageId;
        pInformationDisplayer->setNerve(GET_NERVE_ANON(DisplayInformationForResultAppear));
    }

    mInformationDisplayer->updateNerve();

    bool isNotIdle = !mInformationDisplayer->isNerve(GET_NERVE_ANON(DisplayInformationForResultIdle));

    if (!isNotIdle) {
        if (pNerve != nullptr) {
            setNerve(pNerve);
        } else {
            decideNextNerve();
        }

        return true;
    }

    return false;
}

void StageResultInformer::setCounterPaneShowHide(bool isShowTotalStarPane, bool isShowBestScorePane, bool isShowTotalPiecePane) {
    const char* pPaneName = "TotalStar";

    if (isShowTotalStarPane) {
        MR::showPane(this, pPaneName);
    } else {
        MR::hidePane(this, pPaneName);
    }

    pPaneName = "BestScore";

    if (isShowBestScorePane) {
        MR::showPane(this, pPaneName);
    } else {
        MR::hidePane(this, pPaneName);
    }

    pPaneName = "TotalPiece";

    if (isShowTotalPiecePane) {
        MR::showPane(this, pPaneName);
    } else {
        MR::hidePane(this, pPaneName);
    }
}

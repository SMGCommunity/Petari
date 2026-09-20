#include "Game/System/GameSequenceProgress.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/PlayerMissLeft.hpp"
#include "Game/System/FindingLuigiEventScheduler.hpp"
#include "Game/System/GalaxyCometScheduler.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GalaxyStatusAccessor.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSystem.hpp"
#include "Game/System/GameSystemFunction.hpp"
#include "Game/System/GameSystemSceneController.hpp"
#include "Game/System/LuigiLeftSupplier.hpp"
#include "Game/System/StarPointerOnOffController.hpp"
#include "Game/System/StorySequenceExecutor.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SceneUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SingletonHolder.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"

namespace {
    static const s32 sTimingPlayingTicoSE = 180;
};  // namespace

namespace {
    NEW_NERVE(GameSequenceProgressBooting, GameSequenceProgress, Booting);
    NEW_NERVE(GameSequenceProgressLogo, GameSequenceProgress, Logo);
    NEW_NERVE(GameSequenceProgressWaitGoToFirstScene, GameSequenceProgress, WaitGoToFirstScene);
    NEW_NERVE(GameSequenceProgressNormal, GameSequenceProgress, Normal);
    NEW_NERVE(GameSequenceProgressGalaxyMove, GameSequenceProgress, GalaxyMove);
    NEW_NERVE(GameSequenceProgressResetProcessing, GameSequenceProgress, ResetProcessing);
};  // namespace

GameSequenceProgress::GameSequenceProgress()
    : NerveExecutor("シーケンス進行"), mStarPointerOnOffController(), mStorySequenceExecutor(), mFindingLuigiEventScheduler(),
      mGalaxyCometScheduler(), mLuigiLeftSupplier(), mPlayerMissLeft(), mMinFrame(), mIsPlayTicoSound(), mIsCancelScenarioSelect(),
      mIsForceWipe(true) {
    initNerve(GET_NERVE_ANON(GameSequenceProgressBooting));

    mStarPointerOnOffController = new StarPointerOnOffController();
    mStorySequenceExecutor = new StorySequenceExecutor();
    mFindingLuigiEventScheduler = new FindingLuigiEventScheduler();
    mLuigiLeftSupplier = new LuigiLeftSupplier();
}

void GameSequenceProgress::initAfterResourceLoaded() {
    mGalaxyCometScheduler = new GalaxyCometScheduler();

    mPlayerMissLeft = new PlayerMissLeft("ミス時のプレイヤー残機表示");
    mPlayerMissLeft->initWithoutIter();

    mFindingLuigiEventScheduler->initAfterResourceLoaded();
}

void GameSequenceProgress::update() {
    updateNerve();
    mStarPointerOnOffController->update();
    mStorySequenceExecutor->update();

    if (mGalaxyCometScheduler != nullptr) {
        mGalaxyCometScheduler->update();
    }

    if (!isNerve(GET_NERVE_ANON(GameSequenceProgressResetProcessing)) && GameSystemFunction::isResetProcessing()) {
        setNerve(GET_NERVE_ANON(GameSequenceProgressResetProcessing));
    }

    if (mPlayerMissLeft != nullptr) {
        mPlayerMissLeft->movement();
        mPlayerMissLeft->calcAnim();
    }
}

void GameSequenceProgress::draw() {
    if (mPlayerMissLeft != nullptr) {
        mPlayerMissLeft->draw();
    }
}

void GameSequenceProgress::startScene() {
    mMinFrame = 0;
    mIsPlayTicoSound = false;

    if (MR::isEqualSceneName("Logo")) {
        mStarPointerOnOffController->setStateToBase(this);
    } else if (MR::isEqualSceneName("Title")) {
        mStarPointerOnOffController->setStateToBase(this);
    } else if (MR::isEqualSceneName("ThankYou")) {
        mStarPointerOnOffController->setStateToBase(this);
    } else if (MR::isStageFileSelect()) {
        mStarPointerOnOffController->setStateToBase(this);
    } else {
        mStarPointerOnOffController->setStateToBase(this);
        MR::startStarPointerModeGame(this);
    }

    GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(isScenePermittedIsUpdateWiiRemoteStatus());
    GameSystemFunction::setAutoSleepTimeWiiRemote(isSceneLongAutoSleepWiiRemote());

    if (MR::isEqualSceneName("Game")) {
        mStorySequenceExecutor->setNerveSceneStart();
        MR::startGlobalTimer();

        if (MR::isStageFileSelect()) {
            MR::forceOpenWipeFade();
        }

        if (mIsForceWipe) {
            MR::forceOpenSystemWipeFade();
        }

        if (!GameDataFunction::isPassedStoryEvent("スピン権利")) {
            MR::setPlayerSwingPermission(false);
        }
    }

    if (mPlayerMissLeft != nullptr) {
        mPlayerMissLeft->kill();
    }
}

void GameSequenceProgress::endScene() {
    if (isNerve(GET_NERVE_ANON(GameSequenceProgressLogo))) {
        MR::requestChangeSceneAfterBoot();

        GameSystemFunction::setResetOperationApplicationReset();

        setNerve(GET_NERVE_ANON(GameSequenceProgressWaitGoToFirstScene));
    } else if (!isNerve(GET_NERVE_ANON(GameSequenceProgressWaitGoToFirstScene))) {
        MR::requestChangeSceneTitle();
    }
}

void GameSequenceProgress::requestChangeScene(const char* pName) {
    updateGameDataBeforeChangeScene();

    GameSystemSceneController* pSceneController = SingletonHolder< GameSystem >::get()->mSceneController;
    pSceneController->mNextSceneControlInfo.setScene(pName);
    pSceneController->requestChangeScene();

    mStarPointerOnOffController->setStateToBase(this);
    mStorySequenceExecutor->forceStop();
}

void GameSequenceProgress::requestGalaxyMove(const GalaxyMoveArgument& rMoveArg) {
    updateGameDataGalaxyVisitedFlag();
    GameSequenceFunction::storeSceneStartGameDataHolder();

    if (rMoveArg.mMoveType == 4) {
        GameSequenceFunction::updateGameDataAndSequenceAfterStageResultSequence();
        mFindingLuigiEventScheduler->updateOnStageResult(GameSequenceFunction::getClearedStageName(), GameSequenceFunction::getClearedPowerStarId());

        countDownGameEventValueFromNewPowerStar();
    }

    mFindingLuigiEventScheduler->update(rMoveArg);

    GalaxyMoveArgument moveArg(rMoveArg);
    mStorySequenceExecutor->moveGalaxy(&moveArg, isNerve(GET_NERVE_ANON(GameSequenceProgressResetProcessing)) || mIsCancelScenarioSelect);

    mGalaxyCometScheduler->syncWithFlags();

    updateGameDataBeforeChangeScene();
    setMinFrameBeforeStartNextStage(moveArg);

    GameSystemSceneController* pController = SingletonHolder< GameSystem >::get()->mSceneController;
    pController->mNextSceneControlInfo.setScene("Game");
    pController->mNextSceneControlInfo.setStage(moveArg.mStageName);
    pController->mNextSceneControlInfo.mScenarioNo = moveArg.mScenarioNo;
    pController->mNextSceneControlInfo.mSelectedScenarioNo = moveArg._C;
    pController->mNextSceneControlInfo.setStartIdInfo(moveArg.mIDInfo);
    pController->requestChangeScene();

    resetGameDataAfterChangeScene(moveArg);

    switch (moveArg.mMoveType) {
    case 2:
        SingletonHolder< GameSystem >::get()->mSceneController->startScenarioSelectScene();
        MR::setStarPointerModeBase();

        break;
    case 7:
        SingletonHolder< GameSystem >::get()->mSceneController->startScenarioSelectSceneBackground();
        mStarPointerOnOffController->setStateToTitle(this);
        break;
    case 5:
        if (mPlayerMissLeft != nullptr && !isCometStar(moveArg)) {
            mPlayerMissLeft->appear();
        }
    case 0:
    case 1:
    case 3:
    case 4:
    case 6:
        SingletonHolder< GameSystem >::get()->mSceneController->startScenarioSelectSceneBackground();
        mStarPointerOnOffController->setStateToBase(this);

        if (moveArg.mMoveType == 6) {
            mLuigiLeftSupplier->syncWithFlags();
        }

        break;
    }

    mIsForceWipe = true;

    if (moveArg.mMoveType == 2 || MR::isEqualString(moveArg.mStageName, "EpilogueDemoStage")) {
        mIsForceWipe = false;
    }

    setNerve(GET_NERVE_ANON(GameSequenceProgressGalaxyMove));
}

void GameSequenceProgress::requestCancelScenarioSelect() {
    mIsCancelScenarioSelect = true;
}

GalaxyCometScheduler* GameSequenceProgress::getGalaxyCometScheduler() {
    return mGalaxyCometScheduler;
}

void GameSequenceProgress::exeBooting() {
    if (MR::isFirstStep(this)) {
    }

    if (GameSequenceFunction::isReadyToStartScene()) {
        setNerve(GET_NERVE_ANON(GameSequenceProgressLogo));
    }
}

void GameSequenceProgress::exeLogo() {
    if (MR::isFirstStep(this)) {
        startScene();
        GameSequenceFunction::startScene();

        GameSystemFunction::tryToLoadSystemArchive();
    }
}

void GameSequenceProgress::exeWaitGoToFirstScene() {
    if (GameSequenceFunction::isReadyToStartScene()) {
        setNerve(GET_NERVE_ANON(GameSequenceProgressNormal));
    }
}

void GameSequenceProgress::exeNormal() {
    if (MR::isFirstStep(this)) {
        if (mIsCancelScenarioSelect) {
            requestGalaxyMove(GalaxyMoveArgument(3, nullptr, 1, nullptr));
            mIsCancelScenarioSelect = false;
        } else {
            startScene();
            GameSequenceFunction::startScene();
        }
    } else if (GameSequenceFunction::isReadyToStartScene()) {
        setNerve(GET_NERVE_ANON(GameSequenceProgressNormal));
    }
}

void GameSequenceProgress::exeGalaxyMove() {
    if (mIsPlayTicoSound) {
        if (MR::isStep(this, ::sTimingPlayingTicoSE)) {
            MR::startSystemSE("SE_SY_TICO_WAKE_PLAYER");
        }
    }

    if (MR::isGreaterStep(this, mMinFrame) && GameSequenceFunction::isReadyToStartScene() && MR::isDead(mPlayerMissLeft)) {
        setNerve(GET_NERVE_ANON(GameSequenceProgressNormal));
    }
}

void GameSequenceProgress::exeResetProcessing() {
    if (MR::isFirstStep(this)) {
        mIsCancelScenarioSelect = false;
        mMinFrame = 0;
        mIsPlayTicoSound = false;

        mStorySequenceExecutor->forceStop();

        if (mPlayerMissLeft != nullptr) {
            mPlayerMissLeft->kill();
        }

        mFindingLuigiEventScheduler->clearLostAndFoundCount();
    }

    if (!GameSystemFunction::isResetProcessing()) {
        MR::requestChangeSceneTitle();
        setNerve(GET_NERVE_ANON(GameSequenceProgressWaitGoToFirstScene));
    }
}

bool GameSequenceProgress::isScenePermittedIsUpdateWiiRemoteStatus() {
    return !MR::isEqualSceneName("Logo");
}

bool GameSequenceProgress::isSceneLongAutoSleepWiiRemote() {
    if (MR::isEqualSceneName("Game") && MR::isEqualStageName("EpilogueDemoStage")) {
        return true;
    }

    return false;
}

bool GameSequenceProgress::isCometStar(GalaxyMoveArgument moveArg) {
    GalaxyStatusAccessor accessor = MR::makeGalaxyStatusAccessor(moveArg.mStageName);
    return accessor.isCometStar(moveArg.mScenarioNo);
}

void GameSequenceProgress::updateGameDataBeforeChangeScene() {
    if (GameSequenceFunction::hasStageResultSequence() && GameSequenceFunction::isPowerStarAtResultSequence("KoopaBattleVs3Galaxy", 1)) {
        GameSequenceFunction::reflectStageResultSequenceCoin();

        GameDataFunction::addStockedStarPiece(GameSequenceFunction::getClearedStarPieceNum());

        GameSequenceFunction::resetStageResultSequenceParam();
    }
}

void GameSequenceProgress::resetGameDataAfterChangeScene(const GalaxyMoveArgument& rMoveArg) {
    s32 starPieceNum = GameDataFunction::getStarPieceNum();
    s32 last1upStarPieceNum = GameDataFunction::getLast1upStarPieceNum();

    switch (rMoveArg.mMoveType) {
    case 6:
        mGalaxyCometScheduler->restoreStateFromGameData();

    case 0:
    case 1:
    case 2:
    case 3:
    case 7:
        GameDataFunction::resetGameDataGoToGalaxyFirst();

        break;
    case 5:
        GameDataFunction::resetGameDataGoToGalaxyRetry();

        break;
    }

    if (rMoveArg.isEqualStageScenario("HeavensDoorGalaxy", 1) && mStorySequenceExecutor->isEqualStageScenarioBefore("PeachCastleGardenGalaxy", 1) &&
        rMoveArg.mMoveType == 0) {
        GameDataFunction::addStarPiece(starPieceNum);
        GameDataFunction::setLast1upStarPieceNum(last1upStarPieceNum);
    }
}

void GameSequenceProgress::updateGameDataGalaxyVisitedFlag() {
    if (MR::isEqualSceneName("Game") && !mIsCancelScenarioSelect && !isNerve(GET_NERVE_ANON(GameSequenceProgressResetProcessing))) {
        const char* pStageName = MR::getCurrentStageName();
        s32 scenarioNo = MR::getCurrentScenarioNo();
        GameDataFunction::onGalaxyScenarioFlagAlreadyVisited(pStageName, scenarioNo);
    }
}

void GameSequenceProgress::setMinFrameBeforeStartNextStage(const GalaxyMoveArgument& rMoveArg) {
    if (MR::isEqualSceneName("Game") && MR::isEqualStageName("PeachCastleGardenGalaxy") &&
        MR::isEqualString(rMoveArg.mStageName, "HeavensDoorGalaxy") && rMoveArg.mScenarioNo == 1) {
        mMinFrame = 300;
        mIsPlayTicoSound = true;
    }
}

bool GameSequenceProgress::isLuigiLeftSupply() {
    return mLuigiLeftSupplier->_0;
}

void GameSequenceProgress::offLuigiLeftSupply() {
    mLuigiLeftSupplier->_0 = false;
}

void GameSequenceProgress::countDownGameEventValueFromNewPowerStar() {
    if (!GameSequenceFunction::hasStageResultSequence()) {
        return;
    }

    if (GameSequenceFunction::hasPowerStarYetAtResultSequence()) {
        return;
    }

    for (GameEventFlagIter iter = GameEventFlagTable::getBeginIter(); !iter.isEnd(); iter.goNext()) {
        GameEventFlagAccessor accessor(iter.getFlag());

        if (!accessor.isTypeEventValueIsZero()) {
            continue;
        }

        if (GameDataFunction::canOnGameEventFlag(accessor.getName())) {
            continue;
        }

        if (!GameDataFunction::isOnGameEventFlag(accessor.getRequirement())) {
            continue;
        }

        if (GameDataFunction::isOnJustGameEventFlag(accessor.getRequirement())) {
            continue;
        }

        u32 eventValue = GameDataFunction::getGameEventValue(accessor.getEventValueName());
        GameDataFunction::setGameEventValue(accessor.getEventValueName(), eventValue - 1);
    }
}

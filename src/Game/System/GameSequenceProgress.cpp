#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/PlayerMissLeft.hpp"
#include "Game/System/FindingLuigiEventScheduler.hpp"
#include "Game/System/GalaxyCometScheduler.hpp"
#include "Game/System/GalaxyMoveArgument.hpp"
#include "Game/System/GameDataFunction.hpp"
#include "Game/System/GameEventFlagTable.hpp"
#include "Game/System/GameSequenceFunction.hpp"
#include "Game/System/GameSequenceProgress.hpp"
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
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "Game/SingletonHolder.hpp"

namespace {
    static const s32 sTimingPlayingTicoSE = 180;
};

namespace {
    NEW_NERVE(GameSequenceProgressBooting, GameSequenceProgress, Booting);
    NEW_NERVE(GameSequenceProgressLogo, GameSequenceProgress, Logo);
    NEW_NERVE(GameSequenceProgressWaitGoToFirstScene, GameSequenceProgress, WaitGoToFirstScene);
    NEW_NERVE(GameSequenceProgressNormal, GameSequenceProgress, Normal);
    NEW_NERVE(GameSequenceProgressGalaxyMove, GameSequenceProgress, GalaxyMove);
    NEW_NERVE(GameSequenceProgressResetProcessing, GameSequenceProgress, ResetProcessing);
};

GameSequenceProgress::GameSequenceProgress() :
    NerveExecutor("シーケンス進行"),
    mStarPointerOnOffController(nullptr),
    mStorySequenceExecutor(nullptr),
    mFindingLuigiEventScheduler(nullptr),
    mGalaxyCometScheduler(nullptr),
    mLuigiLeftSupplier(nullptr),
    mPlayerMissLeft(nullptr),
    _20(0),
    _24(false),
    _25(false),
    _26(true)
{
    initNerve(&::GameSequenceProgressBooting::sInstance);

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

    if (!isNerve(&::GameSequenceProgressResetProcessing::sInstance)
        && GameSystemFunction::isResetProcessing())
    {
        setNerve(&::GameSequenceProgressResetProcessing::sInstance);
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
    _20 = 0;
    _24 = false;

    if (MR::isEqualSceneName("Logo")) {
        mStarPointerOnOffController->setStateToBase(this);
    }
    else if (MR::isEqualSceneName("Title")) {
        mStarPointerOnOffController->setStateToBase(this);
    }
    else if (MR::isEqualSceneName("ThankYou")) {
        mStarPointerOnOffController->setStateToBase(this);
    }
    else if (MR::isStageFileSelect()) {
        mStarPointerOnOffController->setStateToBase(this);
    }
    else {
        mStarPointerOnOffController->setStateToBase(this);
        MR::startStarPointerModeGame(this);
    }

    GameSystemFunction::setPermissionToCheckWiiRemoteConnectAndScreenDimming(
        isScenePermittedIsUpdateWiiRemoteStatus());
    GameSystemFunction::setAutoSleepTimeWiiRemote(
        isSceneLongAutoSleepWiiRemote());

    if (MR::isEqualSceneName("Game")) {
        mStorySequenceExecutor->setNerveSceneStart();
        MR::startGlobalTimer();

        if (MR::isStageFileSelect()) {
            MR::forceOpenWipeFade();
        }

        if (_26) {
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
    if (isNerve(&::GameSequenceProgressLogo::sInstance)) {
        MR::requestChangeSceneAfterBoot();
        GameSystemFunction::setResetOperationApplicationReset();
        setNerve(&::GameSequenceProgressWaitGoToFirstScene::sInstance);
    }
    else if (!isNerve(&GameSequenceProgressWaitGoToFirstScene::sInstance)) {
        MR::requestChangeSceneTitle();
    }
}

void GameSequenceProgress::requestChangeScene(const char* pParam1) {
    GameSystemSceneController* pSceneController;

    updateGameDataBeforeChangeScene();

    pSceneController = SingletonHolder<GameSystem>::get()->mSceneController;
    pSceneController->_4C.setScene(pParam1);
    pSceneController->requestChangeScene();

    mStarPointerOnOffController->setStateToBase(this);
    mStorySequenceExecutor->forceStop();
}

// GameSequenceProgress::requestGalaxyMove

void GameSequenceProgress::requestCancelScenarioSelect() {
    _25 = true;
}

GalaxyCometScheduler* GameSequenceProgress::getGalaxyCometScheduler() {
    return mGalaxyCometScheduler;
}

void GameSequenceProgress::exeBooting() {
    if (MR::isFirstStep(this)) {
        
    }

    if (GameSequenceFunction::isReadyToStartScene()) {
        setNerve(&::GameSequenceProgressLogo::sInstance);
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
        setNerve(&::GameSequenceProgressNormal::sInstance);
    }
}

void GameSequenceProgress::exeNormal() {
    if (MR::isFirstStep(this)) {
        if (_25) {
            requestGalaxyMove(GalaxyMoveArgument(3, nullptr, 1, nullptr));

            _25 = false;
        }
        else {
            startScene();
            GameSequenceFunction::startScene();
        }
    }
    else if (GameSequenceFunction::isReadyToStartScene()) {
        setNerve(&::GameSequenceProgressNormal::sInstance);
    }
}

void GameSequenceProgress::exeGalaxyMove() {
    if (_24) {
        if (MR::isStep(this, sTimingPlayingTicoSE)) {
            MR::startSystemSE("SE_SY_TICO_WAKE_PLAYER", -1, -1);
        }
    }

    if (MR::isGreaterStep(this, _20)) {
        if (GameSequenceFunction::isReadyToStartScene()) {
            if (MR::isDead(mPlayerMissLeft)) {
                setNerve(&::GameSequenceProgressNormal::sInstance);
            }
        }
    }
}

void GameSequenceProgress::exeResetProcessing() {
    if (MR::isFirstStep(this)) {
        _25 = false;
        _20 = 0;
        _24 = false;

        mStorySequenceExecutor->forceStop();

        if (mPlayerMissLeft != nullptr) {
            mPlayerMissLeft->kill();
        }

        mFindingLuigiEventScheduler->clearLostAndFoundCount();
    }

    if (GameSystemFunction::isResetProcessing()) {
        return;
    }

    MR::requestChangeSceneTitle();
    setNerve(&::GameSequenceProgressWaitGoToFirstScene::sInstance);
}

bool GameSequenceProgress::isScenePermittedIsUpdateWiiRemoteStatus() {
    return !MR::isEqualSceneName("Logo");
}

bool GameSequenceProgress::isSceneLongAutoSleepWiiRemote() {
    if (MR::isEqualSceneName("Game")
        && MR::isEqualStageName("EpilogueDemoStage"))
    {
        return true;
    }

    return false;
}

void GameSequenceProgress::updateGameDataBeforeChangeScene() {
    if (GameSequenceFunction::hasStageResultSequence()
        && GameSequenceFunction::isPowerStarAtResultSequence("KoopaBattleVs3Galaxy", 1))
    {
        GameSequenceFunction::reflectStageResultSequenceCoin();
        GameDataFunction::addStockedStarPiece(
            GameSequenceFunction::getClearedStarPieceNum());
        GameSequenceFunction::resetStageResultSequenceParam();
    }
}

void GameSequenceProgress::resetGameDataAfterChangeScene(const GalaxyMoveArgument& rParam1) {
    s32 starPieceNum = GameDataFunction::getStarPieceNum();
    s32 last1upStarPieceNum = GameDataFunction::getLast1upStarPieceNum();

    switch (rParam1.mMoveType) {
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

    if (rParam1.isEqualStageScenario("HeavensDoorGalaxy", 1)
        && mStorySequenceExecutor->isEqualStageScenarioBefore("PeachCastleGardenGalaxy", 1)
        && rParam1.mMoveType == 0)
    {
        GameDataFunction::addStarPiece(starPieceNum);
        GameDataFunction::setLast1upStarPieceNum(last1upStarPieceNum);
    }
}

void GameSequenceProgress::updateGameDataGalaxyVisitedFlag() {
    if (MR::isEqualSceneName("Game")
        && !_25
        && !isNerve(&::GameSequenceProgressResetProcessing::sInstance))
    {
        const char* pStageName = MR::getCurrentStageName();
        s32 scenarioNo = MR::getCurrentScenarioNo();

        GameDataFunction::onGalaxyScenarioFlagAlreadyVisited(
            pStageName,
            scenarioNo);
    }
}

void GameSequenceProgress::setMinFrameBeforeStartNextStage(const GalaxyMoveArgument& rParam1) {
    if (MR::isEqualSceneName("Game")
        && MR::isEqualStageName("PeachCastleGardenGalaxy")
        && MR::isEqualString(rParam1.mStageName, "HeavensDoorGalaxy")
        && rParam1.mScenarioNo == 1)
    {
        _20 = 300;
        _24 = true;
    }
}

bool GameSequenceProgress::isLuigiLeftSupply() {
    return mLuigiLeftSupplier->_0;
}

void GameSequenceProgress::offLuigiLeftSupply() {
    mLuigiLeftSupplier->_0 = false;
}

void GameSequenceProgress::countDownGameEventValueFromNewPowerStar() {
    if (GameSequenceFunction::hasStageResultSequence()
        && !GameSequenceFunction::hasPowerStarYetAtResultSequence())
    {
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

            u16 eventValue = GameDataFunction::getGameEventValue(accessor.getEventValueName());

            GameDataFunction::setGameEventValue(
                accessor.getEventValueName(),
                eventValue - 1);
        }
    }
}

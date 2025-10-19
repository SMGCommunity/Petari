#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CoinCounter.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Screen/MarioMeter.hpp"
#include "Game/Screen/PlayerLeft.hpp"
#include "Game/Screen/StarCounter.hpp"
#include "Game/Screen/StarPieceCounter.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    const s32 cJudgeNotMovingFrame = 60;
    const s32 cShowWaitFrame = 240;
};

namespace NrvCounterLayoutController {
    NEW_NERVE(CounterLayoutControllerNrvPlayerMoving, CounterLayoutController, PlayerMoving);
    NEW_NERVE(CounterLayoutControllerNrvPlayerNotMoving, CounterLayoutController, PlayerNotMoving);
};

CounterLayoutController::CounterLayoutController() :
    LayoutActor("カウンタ系レイアウト制御", true),
    mPlayerNotMovingFrame(0),
    _24(false),
    mCoinCounter(NULL),
    mStarPieceCounter(NULL),
    mPlayerLeft(NULL),
    mStarCounter(NULL),
    mHPMeter(NULL)
{}

void CounterLayoutController::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerMoving::sInstance);

    mCoinCounter = new CoinCounter("コインカウンタ");
    mCoinCounter->initWithoutIter();
    mStarPieceCounter = new StarPieceCounter("スターピースカウンタ");
    mStarPieceCounter->initWithoutIter();
    mPlayerLeft = new PlayerLeft("プレイヤー残機表示");
    mPlayerLeft->initWithoutIter();
    mStarCounter = new StarCounter();
    mStarCounter->initWithoutIter();

    appear();
}

void CounterLayoutController::appear() {
    if (_24) {
        mStarPieceCounter->appear();
    }
    else {
        appearAllCounterWithoutStar();
    }

    setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerMoving::sInstance);
    LayoutActor::appear();
}

void CounterLayoutController::kill() {
    LayoutActor::kill();
    killAllCoounter();
}

void CounterLayoutController::forceAppear() {
    setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerNotMoving::sInstance);
}

void CounterLayoutController::setHPMeter(MarioMeter* pHPMeter) {
    mHPMeter = pHPMeter;
}

void CounterLayoutController::requestedTicoEat(bool param1) {
    if (mStarPieceCounter->mMode == 0 && mStarPieceCounter->tryOnModeTicoEat(param1)) {
        mCoinCounter->kill();
        mPlayerLeft->kill();
        mStarCounter->kill();
    }

    _24 = true;
}

bool CounterLayoutController::isInvalidSystemStateShowCounter() {
    return MR::isExistStarPointerGuidance() || MR::isPlayerInBind() || MR::isSystemTalking();
}

bool CounterLayoutController::isWaitToDisappearCounter(const LayoutActor* pActor) {
    return MR::isGreaterStep(pActor, cShowWaitFrame) || MR::isExistStarPointerGuidance();
}

void CounterLayoutController::control() {
    _24 = false;
}

void CounterLayoutController::showAllLayout() {
    if (!_24) {
        mCoinCounter->forceAppear();
        mStarPieceCounter->forceAppear(false);
        mPlayerLeft->forceAppear();
        mStarCounter->appear();
    }
}

void CounterLayoutController::hideAllLayout() {
    if (!_24) {
        mCoinCounter->disappear();
        mStarPieceCounter->disappear(false);
        mPlayerLeft->disappear();
        mStarCounter->disappear();
    }
}

/*
bool CounterLayoutController::isHiddenOrWaitAllLayout() const {
    return mCoinCounter->isHiddenOrWait()
        && mStarPieceCounter->isHiddenOrWait()
        && mPlayerLeft->isHiddenOrWait()
        && mStarCounter->isHiddenOrWait();
}
*/

bool CounterLayoutController::isPlayerMoving() const {
    if (!MR::isPlayerElementModeHopper()
        && !MR::isPlayerElementModeTeresa()
        && !MR::isPlayerSwimming()
        && !MR::isOnGroundPlayer())
    {
        return true;
    }

    if (!MR::isNearZero(MR::getSubPadStickX(0), 0.001f)
        || !MR::isNearZero(MR::getSubPadStickY(0), 0.001f)
        || MR::testPadButtonAnyWithoutHome(0)
        || MR::isCorePadSwing(0)
        || MR::isSubPadSwing(0))
    {
        return true;
    }

    if (isNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerMoving::sInstance)
        && !isHiddenOrWaitAllLayout())
    {
        return true;
    }

    if (isInvalidSystemStateShowCounter()) {
        return true;
    }

    return false;
}

void CounterLayoutController::appearAllCounterWithoutStar() {
    mCoinCounter->appear();
    mStarPieceCounter->appear();
    mPlayerLeft->appear();
}

void CounterLayoutController::killAllCoounter() {
    mCoinCounter->kill();
    mStarPieceCounter->kill();
    mPlayerLeft->kill();
    mStarCounter->kill();
}

bool CounterLayoutController::tryEndTicoEatStarPiece() {
    if (mStarPieceCounter->mMode != 0 && !_24) {
        if (mStarPieceCounter->tryOffModeTicoEat()) {
            mCoinCounter->appear();
            mPlayerLeft->appear();
        }

        return true;
    }

    return false;
}

void CounterLayoutController::exePlayerMoving() {
    if (MR::isFirstStep(this)) {
        mPlayerNotMovingFrame = 0;
    }

    mHPMeter->requestPlayerMoving();

    if (tryEndTicoEatStarPiece()) {
        setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerMoving::sInstance);
    }
    else if (!isPlayerMoving()) {
        mPlayerNotMovingFrame++;

        if (mPlayerNotMovingFrame > cJudgeNotMovingFrame) {
            setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerNotMoving::sInstance);
        }
    }
    else {
        mPlayerNotMovingFrame = 0;
    }
}

void CounterLayoutController::exePlayerNotMoving() {
    if (MR::isFirstStep(this)) {
        showAllLayout();
    }

    mHPMeter->requestPlayerStopped();

    if (tryEndTicoEatStarPiece()) {
        setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerNotMoving::sInstance);
    }
    else if (isPlayerMoving() && isWaitToDisappearCounter(this)) {
        hideAllLayout();
        setNerve(&NrvCounterLayoutController::CounterLayoutControllerNrvPlayerMoving::sInstance);
    }
}

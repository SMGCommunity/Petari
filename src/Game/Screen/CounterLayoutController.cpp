#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CoinCounter.hpp"
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
};  // namespace

namespace NrvCounterLayoutController {
    NEW_NERVE(CounterLayoutControllerNrvPlayerMoving, CounterLayoutController, PlayerMoving);
    NEW_NERVE(CounterLayoutControllerNrvPlayerNotMoving, CounterLayoutController, PlayerNotMoving);
};  // namespace NrvCounterLayoutController

CounterLayoutController::CounterLayoutController()
    : LayoutActor("カウンタ系レイアウト制御", true), mPlayerNotMovingFrame(), _24(), mCoinCounter(), mStarPieceCounter(), mPlayerLeft(),
      mStarCounter(), mHPMeter() {
}

void CounterLayoutController::init(const JMapInfoIter& rIter) {
    MR::connectToSceneLayout(this);
    initNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerMoving));

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
    } else {
        appearAllCounterWithoutStar();
    }

    setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerMoving));
    LayoutActor::appear();
}

void CounterLayoutController::kill() {
    LayoutActor::kill();
    killAllCoounter();
}

void CounterLayoutController::forceAppear() {
    setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerNotMoving));
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
    return MR::isGreaterStep(pActor, ::cShowWaitFrame) || MR::isExistStarPointerGuidance();
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

namespace {
    template < typename T >
    bool isHiddenOrWait(const T* pLayout) {
        return MR::isDead(pLayout) || MR::isHiddenLayout(pLayout) || pLayout->isWait();
    }
}  // namespace

bool CounterLayoutController::isHiddenOrWaitAllLayout() const {
    return ::isHiddenOrWait(mCoinCounter) && ::isHiddenOrWait(mStarPieceCounter) && ::isHiddenOrWait(mPlayerLeft) && ::isHiddenOrWait(mStarCounter);
}

bool CounterLayoutController::isPlayerMoving() const {
    if (!MR::isPlayerElementModeHopper() && !MR::isPlayerElementModeTeresa() && !MR::isPlayerSwimming() && !MR::isOnGroundPlayer()) {
        return true;
    }

    if (!MR::isNearZero(MR::getSubPadStickX(WPAD_CHAN0)) || !MR::isNearZero(MR::getSubPadStickY(WPAD_CHAN0)) ||
        MR::testPadButtonAnyWithoutHome(WPAD_CHAN0) || MR::isCorePadSwing(WPAD_CHAN0) || MR::isSubPadSwing(WPAD_CHAN0)) {
        return true;
    }

    if (isNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerMoving)) && !isHiddenOrWaitAllLayout()) {
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
        setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerMoving));
    } else if (!isPlayerMoving()) {
        mPlayerNotMovingFrame++;

        if (mPlayerNotMovingFrame > ::cJudgeNotMovingFrame) {
            setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerNotMoving));
        }
    } else {
        mPlayerNotMovingFrame = 0;
    }
}

void CounterLayoutController::exePlayerNotMoving() {
    if (MR::isFirstStep(this)) {
        showAllLayout();
    }

    mHPMeter->requestPlayerStopped();

    if (tryEndTicoEatStarPiece()) {
        setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerNotMoving));
    } else if (isPlayerMoving() && isWaitToDisappearCounter(this)) {
        hideAllLayout();
        setNerve(GET_NERVE(CounterLayoutController, CounterLayoutControllerNrvPlayerMoving));
    }
}

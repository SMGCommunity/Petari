#include "Game/Screen/WipeGameOver.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"

namespace NrvWipeGameOver {
    NEW_NERVE(WipeGameOverWait, WipeGameOver, Wait);
    NEW_NERVE(WipeGameOverActive, WipeGameOver, Active);
};  // namespace NrvWipeGameOver

WipeGameOver::WipeGameOver() : WipeLayoutBase("ゲームオーバー") {}

void WipeGameOver::init(const JMapInfoIter& rIter) {
    initNerve(&NrvWipeGameOver::WipeGameOverWait::sInstance);
    initLayoutManager("GameOver", 1);
}

void WipeGameOver::kill() {
    LayoutActor::kill();
    setNerve(&NrvWipeGameOver::WipeGameOverWait::sInstance);
}

void WipeGameOver::wipe(s32 step) {
    if (isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance)) {
        return;
    }

    appear();
    MR::hideLayout(this);
    setNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance);
}

void WipeGameOver::forceClose() {
    wipe(-1);
}

void WipeGameOver::forceOpen() {
    MR::hideLayout(this);
    kill();
}

bool WipeGameOver::isOpen() const {
    return MR::isDead(this) || isNerve(&NrvWipeGameOver::WipeGameOverWait::sInstance);
}

bool WipeGameOver::isClose() const {
    return isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance) && MR::isAnimStopped(this, 0);
}

bool WipeGameOver::isWipeIn() const {
    return false;
}

bool WipeGameOver::isWipeOut() const {
    return isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance) && !MR::isAnimStopped(this, 0);
}

void WipeGameOver::exeWait() {}

void WipeGameOver::exeActive() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "GameOver", 0);
    }
}

#include "Game/Screen/WipeGameOver.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvWipeGameOver {
    NERVE_DECL_EXE(WipeGameOverActive, WipeGameOver, Active);
    NERVE_DECL_EXE(WipeGameOverWait, WipeGameOver, Wait);

    INIT_NERVE(WipeGameOverWait);
    INIT_NERVE(WipeGameOverActive);
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

void WipeGameOver::wipe(s32 a1) {
    if (!isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance)) {
        appear();
        MR::hideLayout(this);
        setNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance);
    }
}

void WipeGameOver::forceClose() {
    wipe(-1);
}

void WipeGameOver::forceOpen() {
    MR::hideLayout(this);
    kill();
}

bool WipeGameOver::isOpen() const {
    bool ret = false;
    if (MR::isDead(this) || isNerve(&NrvWipeGameOver::WipeGameOverWait::sInstance)) {
        ret = true;
    }

    return ret;
}

bool WipeGameOver::isClose() const {
    bool ret = false;
    if (isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance) && MR::isAnimStopped(this, 0)) {
        ret = true;
    }
    return ret;
}

bool WipeGameOver::isWipeIn() const {
    return false;
}

bool WipeGameOver::isWipeOut() const {
    bool ret = false;
    if (isNerve(&NrvWipeGameOver::WipeGameOverActive::sInstance) && !MR::isAnimStopped(this, 0)) {
        ret = true;
    }
    return ret;
}

WipeGameOver::~WipeGameOver() {}

void WipeGameOver::exeActive() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        MR::startAnim(this, "GameOver", 0);
    }
}

void WipeGameOver::exeWait() {}

s32 WipeGameOver::getWipeType() const {
    return 2;
}
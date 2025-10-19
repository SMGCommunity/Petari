#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/BombTimerLayout.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvBombTimerLayout {
    NEW_NERVE(BombTimerLayoutNrvAppear, BombTimerLayout, Appear);
    NEW_NERVE(BombTimerLayoutNrvWait, BombTimerLayout, Wait);
    NEW_NERVE(BombTimerLayoutNrvDanger, BombTimerLayout, Danger);
    NEW_NERVE(BombTimerLayoutNrvEnd, BombTimerLayout, End);
};

BombTimerLayout::BombTimerLayout(bool param1) :
    LayoutActor("ボムタイマーレイアウト", true),
    mDangerTime(600),
    mTime(0),
    mIsSuspend(false)
{
    if (param1) {
        MR::connectToSceneLayout(this);
    }
}

void BombTimerLayout::init(const JMapInfoIter& rIter) {
    initLayoutManager("BombTimer", 2);
    initNerve(&NrvBombTimerLayout::BombTimerLayoutNrvAppear::sInstance);
    kill();
}

void BombTimerLayout::appear() {
    mTime = 0;

    LayoutActor::appear();
    setNerve(&NrvBombTimerLayout::BombTimerLayoutNrvAppear::sInstance);
}

void BombTimerLayout::setTimeLimit(u32 timeLimit) {
    mTimeLimit = timeLimit;

    updateTextBox();
}

void BombTimerLayout::suspend() {
    mIsSuspend = true;
}

void BombTimerLayout::resume() {
    mIsSuspend = false;
}

bool BombTimerLayout::isReadyToTimeUp() const {
    return mTime < mTimeLimit == false;
}

void BombTimerLayout::addFrame() {
    if (isReadyToTimeUp()) {
        return;
    }

    mTime++;

    updateTextBox();
}

bool BombTimerLayout::update() {
    if (mIsSuspend) {
        return false;
    }

    addFrame();

    if (isReadyToTimeUp()) {
        setNerve(&NrvBombTimerLayout::BombTimerLayoutNrvEnd::sInstance);

        return true;
    }

    return false;
}

u32 BombTimerLayout::getRestTime() const {
    return mTimeLimit - mTime;
}

void BombTimerLayout::updateTextBox() {
    MR::setTextBoxNumberRecursive(this, "Counter", (getRestTime() + 59) / 60);
}

void BombTimerLayout::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Appear", 0);
    }

    update();

    if (MR::isAnimStopped(this, 0)) {
        setNerve(&NrvBombTimerLayout::BombTimerLayoutNrvWait::sInstance);
    }
}

void BombTimerLayout::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Wait", 0);
    }

    update();

    if (getRestTime() < mDangerTime) {
        setNerve(&NrvBombTimerLayout::BombTimerLayoutNrvDanger::sInstance);
    }
}

void BombTimerLayout::exeDanger() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "Danger", 0);
        MR::startAnim(this, "Blink", 1);
    }

    update();
}

void BombTimerLayout::exeEnd() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(this, "End", 0);
    }

    if (MR::isAnimStopped(this, 0)) {
        kill();
    }
}

#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/StarCounter.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/SceneUtil.hpp"

namespace NrvStarCounter {
    NEW_NERVE(StarCounterNrvAppear, StarCounter, Appear);
    NEW_NERVE(StarCounterNrvWait, StarCounter, Wait);
    NEW_NERVE(StarCounterNrvCountUp, StarCounter, CountUp);
    NEW_NERVE(StarCounterNrvDisappear, StarCounter, Disappear);
}; // namespace NrvStarCounter

StarCounter::StarCounter()
    : LayoutActor("スターカウンタ", true),
      mPowerStarNum(0),
      mLayoutAppearer(nullptr),
      mPaneRumbler(nullptr) {}

void StarCounter::init(const JMapInfoIter& rIter) {
    initLayoutManager("StarCounter", 2);
    MR::createAndAddPaneCtrl(this, "Counter", 1);

    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));
    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");

    MR::connectToSceneLayout(this);
    initNerve(&NrvStarCounter::StarCounterNrvAppear::sInstance);
}

void StarCounter::appear() {
    mLayoutAppearer->appear(TVec2f(0.0f, 0.0f));
    mPaneRumbler->reset();
    MR::startAnim(this, "Wait", 1);

    mPowerStarNum = MR::getPowerStarNum();

    MR::setTextBoxNumberRecursive(this, "Counter", mPowerStarNum);
    setNerve(&NrvStarCounter::StarCounterNrvAppear::sInstance);
    LayoutActor::appear();
    MR::requestMovementOn(this);
}

void StarCounter::disappear() {
    MR::requestMovementOn(this);
    setNerve(&NrvStarCounter::StarCounterNrvDisappear::sInstance);
}

bool StarCounter::isWait() const {
    return isNerve(&NrvStarCounter::StarCounterNrvWait::sInstance);
}

void StarCounter::control() {
    mLayoutAppearer->update();
    mPaneRumbler->update();
}

void StarCounter::exeAppear() {
    if (mLayoutAppearer->isAppeared()) {
        setNerve(&NrvStarCounter::StarCounterNrvWait::sInstance);
    }
}

void StarCounter::exeWait() {
    if (mPowerStarNum < MR::getPowerStarNum()) {
        setNerve(&NrvStarCounter::StarCounterNrvCountUp::sInstance);
    }
}

void StarCounter::exeCountUp() {
    if (MR::isFirstStep(this)) {
        mPowerStarNum++;

        MR::setTextBoxNumberRecursive(this, "Counter", mPowerStarNum);
        MR::startAnim(this, "Flash", 0);
        mPaneRumbler->start();
    }

    if (MR::isAnimStopped(this, 0) && !mPaneRumbler->isRumbling()) {
        setNerve(&NrvStarCounter::StarCounterNrvWait::sInstance);
    }
}

void StarCounter::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mLayoutAppearer->disappear();
    }

    if (mLayoutAppearer->isDisappeared()) {
        kill();
    }
}

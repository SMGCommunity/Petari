#include "Game/Screen/PlayerLeft.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Screen/CounterLayoutController.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvPlayerLeft {
    NEW_NERVE(PlayerLeftNrvHide, PlayerLeft, Hide);
    NEW_NERVE(PlayerLeftNrvAppear, PlayerLeft, Appear);
    NEW_NERVE(PlayerLeftNrvWait, PlayerLeft, Wait);
    NEW_NERVE(PlayerLeftNrvDisappear, PlayerLeft, Disappear);
};  // namespace NrvPlayerLeft

PlayerLeft::PlayerLeft(const char* pName)
    : LayoutActor(pName, true), mPlayerLeft(0), mLayoutAppearer(nullptr), mPaneRumbler(nullptr), mIsForceAppear(false) {
}

void PlayerLeft::init(const JMapInfoIter& rIter) {
    initLayoutManager("PlayerLeft", 3);
    MR::createAndAddPaneCtrl(this, "Counter", 1);
    MR::hidePaneRecursive(this, "CenterPlayerLeft");

    mLayoutAppearer = new CounterLayoutAppearer(this, TVec2f(-50.0f, 0.0f));
    mPaneRumbler = new CountUpPaneRumbler(this, "Counter");
    mPlayerLeft = MR::getPlayerLeft();

    if (MR::isPlayerLuigi()) {
        MR::startAnim(this, "Luigi", 2);
    }

    initNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvHide));
    MR::connectToSceneLayout(this);
    appear();
}

void PlayerLeft::appear() {
    mLayoutAppearer->reset();
    mPaneRumbler->reset();

    mIsForceAppear = false;

    MR::hideLayout(this);
    setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvHide));
    LayoutActor::appear();
}

void PlayerLeft::forceAppear() {
    if (!isNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvWait))) {
        appear();
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvAppear));
    }

    mIsForceAppear = true;
}

void PlayerLeft::disappear() {
    mIsForceAppear = false;

    setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvDisappear));
}

bool PlayerLeft::isWait() const {
    return isNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvWait));
}

void PlayerLeft::control() {
    updateCounter();
    mLayoutAppearer->update();
    mPaneRumbler->update();
}

void PlayerLeft::updateCounter() {
    MR::setTextBoxNumberRecursive(this, "Counter", mPlayerLeft);
}

bool PlayerLeft::isValidAppearSituation() const {
    return mPlayerLeft < MR::getPlayerLeft();
}

void PlayerLeft::exeHide() {
    if (MR::isFirstStep(this)) {
        MR::hideLayout(this);
    }

    if (isValidAppearSituation()) {
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvAppear));
    }
}

void PlayerLeft::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::showLayout(this);
        mLayoutAppearer->appear(TVec2f(0.0f, 0.0f));
        MR::startAnim(this, "Wait", 1);
    }

    if (mLayoutAppearer->isAppeared()) {
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvWait));
    }
}

void PlayerLeft::exeWait() {
    if (MR::isFirstStep(this)) {
        if (isValidAppearSituation()) {
            mPlayerLeft++;
            mPaneRumbler->start();
            MR::startAnim(this, "Flash", 0);
        }
    }

    if (mPaneRumbler->isRumbling()) {
        return;
    }

    if (isValidAppearSituation()) {
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvWait));
    } else if (!mIsForceAppear && CounterLayoutController::isWaitToDisappearCounter(this)) {
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvDisappear));
    }
}

void PlayerLeft::exeDisappear() {
    if (MR::isFirstStep(this)) {
        mLayoutAppearer->disappear();
    }

    if (mLayoutAppearer->isDisappeared()) {
        setNerve(GET_NERVE(PlayerLeft, PlayerLeftNrvHide));
    }
}

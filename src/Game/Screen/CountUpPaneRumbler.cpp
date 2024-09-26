#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "math_types.hpp"

CountUpPaneRumbler::CountUpPaneRumbler(LayoutActor *pActor, const char *pName) {
    mRumbleCalculator = nullptr;
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
    mRumbleCalculator = new RumbleCalculatorCosMultLinear(2.0f, HALF_PI, 8.0f, 0xF);
    MR::createAndAddPaneCtrl(pActor, pName, 1);
    MR::setFollowPos(&mFollowPos, pActor, pName);
    MR::setFollowTypeAdd(pActor, pName);
}

void CountUpPaneRumbler::update() {
    mRumbleCalculator->calc();
    f32 y = mRumbleCalculator->_C.y;
    mFollowPos.x = 0.0f;
    mFollowPos.y = y;
}

void CountUpPaneRumbler::start() {
    mRumbleCalculator->start(0);
}

void CountUpPaneRumbler::reset() {
    mRumbleCalculator->reset();
}

bool CountUpPaneRumbler::isRumbling() const {
    return (mRumbleCalculator->_4 < mRumbleCalculator->_8);
}

void CountUpPaneRumbler::connectPane(LayoutActor *pActor, const char *pName) {
    MR::createAndAddPaneCtrl(pActor, pName, 1);
    MR::setFollowPos(&mFollowPos, pActor, pName);
    MR::setFollowTypeAdd(pActor, pName);
}
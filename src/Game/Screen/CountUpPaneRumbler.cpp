#include "Game/Screen/CountUpPaneRumbler.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/RumbleCalculator.hpp"
#include "math_types.hpp"

namespace {
    const f32 cRumbleFreq = 2.0f;
    const f32 cRumbleAmp = 8.0f;
    const s32 cRumbleFrame = 15;
};  // namespace

CountUpPaneRumbler::CountUpPaneRumbler(LayoutActor* pHost, const char* pPaneName) : mRumbleCalculator(nullptr), mPosition(0.0f, 0.0f) {
    mRumbleCalculator = new RumbleCalculatorCosMultLinear(cRumbleFreq, HALF_PI, cRumbleAmp, cRumbleFrame);

    connectPane(pHost, pPaneName);
}

void CountUpPaneRumbler::update() {
    mRumbleCalculator->calc();

    f32 y = mRumbleCalculator->_C.y;
    mPosition.x = 0.0f;
    mPosition.y = y;
}

void CountUpPaneRumbler::start() {
    mRumbleCalculator->start(0);
}

void CountUpPaneRumbler::reset() {
    mRumbleCalculator->reset();
}

bool CountUpPaneRumbler::isRumbling() const {
    return mRumbleCalculator->_4 < mRumbleCalculator->_8;
}

void CountUpPaneRumbler::connectPane(LayoutActor* pHost, const char* pPaneName) {
    MR::createAndAddPaneCtrl(pHost, pPaneName, 1);
    MR::setFollowPos(&mPosition, pHost, pPaneName);
    MR::setFollowTypeAdd(pHost, pPaneName);
}

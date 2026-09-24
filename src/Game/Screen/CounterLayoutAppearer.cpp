#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

void CounterLayoutAppearer_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

namespace NrvCounterLayoutAppearer {
    NEW_NERVE(CounterLayoutAppearerNrvHide, CounterLayoutAppearer, Hide);
    NEW_NERVE(CounterLayoutAppearerNrvAppear, CounterLayoutAppearer, Appear);
    NEW_NERVE(CounterLayoutAppearerNrvShow, CounterLayoutAppearer, Show);
    NEW_NERVE(CounterLayoutAppearerNrvDisappear, CounterLayoutAppearer, Disappear);
}  // namespace NrvCounterLayoutAppearer

CounterLayoutAppearer::CounterLayoutAppearer(LayoutActor* pActor, const TVec2f& rAppearOffset)
    : NerveExecutor("カウンタ出現制御"), mLayout(pActor), mAppearOffset(rAppearOffset), mFollowPos(0.0f, 0.0f), _1C(0.0f, 0.0f) {
    MR::setFollowPos(&mFollowPos, pActor, nullptr);
    MR::setFollowTypeAdd(mLayout, nullptr);
    initNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvHide));
    MR::startAnimAndSetFrameAndStop(mLayout, "Appear", 0.0f, 0);
    setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvHide));
}

void CounterLayoutAppearer::update() {
    updateNerve();
}

void CounterLayoutAppearer::appear(const TVec2f& rOffset) {
    _1C.x = rOffset.x;
    _1C.y = rOffset.y;

    MR::startAnim(mLayout, "Appear", 0);
    updateLayoutOffset(0.0f);
    setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvAppear));
}

void CounterLayoutAppearer::disappear() {
    setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvDisappear));
}

bool CounterLayoutAppearer::isAppeared() const {
    return isNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvShow));
}

bool CounterLayoutAppearer::isDisappeared() const {
    return isNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvHide));
}

void CounterLayoutAppearer::reset() {
    MR::startAnimAndSetFrameAndStop(mLayout, "Appear", 0.0f, 0);
    setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvHide));
}

void CounterLayoutAppearer::updateLayoutOffset(f32 offset) {
    TVec2f zero(0.0f, 0.0f);
    mFollowPos.x = mAppearOffset.x + offset * (zero.x - mAppearOffset.x);
    mFollowPos.y = mAppearOffset.y + offset * (zero.y - mAppearOffset.y);
    mFollowPos.add(_1C);
}

void CounterLayoutAppearer::exeHide() {
}

void CounterLayoutAppearer::exeAppear() {
    updateLayoutOffset(MR::calcNerveEaseOutRate(this, MR::getAnimFrameMax(mLayout, (u32)0)));

    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvShow));
    }
}

void CounterLayoutAppearer::exeShow() {
    if (MR::isFirstStep(this)) {
        updateLayoutOffset(1.0f);
    }
}

void CounterLayoutAppearer::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mLayout, "End", 0);
    }

    updateLayoutOffset(1.0f - MR::calcNerveEaseInRate(this, MR::getAnimFrameMax(mLayout, static_cast< u32 >(0))));

    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(GET_NERVE(CounterLayoutAppearer, CounterLayoutAppearerNrvHide));
    }
}

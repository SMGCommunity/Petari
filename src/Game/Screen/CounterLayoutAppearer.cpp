#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace NrvCounterLayoutAppearer {
    NEW_NERVE(CounterLayoutAppearerNrvHide, CounterLayoutAppearer, Hide);
    NEW_NERVE(CounterLayoutAppearerNrvAppear, CounterLayoutAppearer, Appear);
    NEW_NERVE(CounterLayoutAppearerNrvShow, CounterLayoutAppearer, Show);
    NEW_NERVE(CounterLayoutAppearerNrvDisappear, CounterLayoutAppearer, Disappear);
};  // namespace NrvCounterLayoutAppearer

CounterLayoutAppearer::CounterLayoutAppearer(LayoutActor* pActor, const TVec2f& rAppearOffset)
    : NerveExecutor("カウンタ出現制御"), mLayout(pActor), mAppearOffset(rAppearOffset.x, rAppearOffset.y), mFollowPos(0.0f, 0.0f), _1C(0.0f, 0.0f) {
    MR::setFollowPos(&mFollowPos, pActor, nullptr);
    MR::setFollowTypeAdd(mLayout, nullptr);
    initNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
    MR::startAnimAndSetFrameAndStop(mLayout, "Appear", 0.0f, 0);
    setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
}

void CounterLayoutAppearer::update() {
    updateNerve();
}

void CounterLayoutAppearer::appear(const TVec2f& param1) {
    _1C.x = param1.x;
    _1C.y = param1.y;

    MR::startAnim(mLayout, "Appear", 0);
    updateLayoutOffset(0.0f);
    setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvAppear::sInstance);
}

void CounterLayoutAppearer::disappear() {
    setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvDisappear::sInstance);
}

bool CounterLayoutAppearer::isAppeared() const {
    return isNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvShow::sInstance);
}

bool CounterLayoutAppearer::isDisappeared() const {
    return isNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
}

void CounterLayoutAppearer::reset() {
    MR::startAnimAndSetFrameAndStop(mLayout, "Appear", 0.0f, 0);
    setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
}

#ifdef NON_MATCHING
void CounterLayoutAppearer::updateLayoutOffset(f32 offset) {
    static f32 sZero = 0.0f;

    TVec2f vec;
    vec.x = sZero - mAppearOffset.x;
    vec.y = sZero - mAppearOffset.y;

    mFollowPos.x = mAppearOffset.x + offset * vec.x + _1C.x;
    mFollowPos.y = mAppearOffset.y + offset * vec.y + _1C.y;
}
#endif

void CounterLayoutAppearer::exeHide() {}

void CounterLayoutAppearer::exeAppear() {
    updateLayoutOffset(MR::calcNerveEaseOutRate(this, MR::getAnimFrameMax(mLayout, (u32)0)));

    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvShow::sInstance);
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

    updateLayoutOffset(1.0f - MR::calcNerveEaseInRate(this, MR::getAnimFrameMax(mLayout, static_cast<u32>(0))));

    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
    }
}

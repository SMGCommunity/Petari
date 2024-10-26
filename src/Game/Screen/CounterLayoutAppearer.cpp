#include "Game/Screen/CounterLayoutAppearer.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

static f32 zero = 0.0f;

CounterLayoutAppearer::CounterLayoutAppearer(LayoutActor *pActor, const TVec2f &a2) : NerveExecutor("カウンタ出現制御") {
    mLayout = pActor;
    _C = a2.x;
    _10 = a2.y;
    mFollowPos.x = 0.0f;
    mFollowPos.y = 0.0f;
    _1C = 0.0f;
    _20 = 0.0f;
    MR::setFollowPos(&mFollowPos, pActor, nullptr);
    MR::setFollowTypeAdd(mLayout, nullptr);
    initNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
    MR::startAnimAndSetFrameAndStop(mLayout, "Appear", 0.0f, 0);
    setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
}

void CounterLayoutAppearer::update() {
    updateNerve();
}

void CounterLayoutAppearer::appear(const TVec2f &a1) {
    _1C = a1.x;
    _20 = a1.y;
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
    TVec2f vec;
    f32 x = (_C + (offset * (zero - _C))) + _1C;
    vec.x = zero;
    vec.y = zero;
    f32 y = ((_10 + (offset * (zero - _10))) + _20);
    mFollowPos.x = x;
    mFollowPos.y = y;
}
#endif

void CounterLayoutAppearer::exeAppear() {
    updateLayoutOffset(MR::calcNerveEaseOutRate(this, MR::getAnimFrameMax(mLayout, (u32)0)));
    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvShow::sInstance);
    }
}

void CounterLayoutAppearer::exeDisappear() {
    if (MR::isFirstStep(this)) {
        MR::startAnim(mLayout, "End", 0);
    }

    updateLayoutOffset((1.0f - MR::calcNerveEaseInRate(this, MR::getAnimFrameMax(mLayout, (u32)0))));

    if (MR::isAnimStopped(mLayout, 0)) {
        setNerve(&NrvCounterLayoutAppearer::CounterLayoutAppearerNrvHide::sInstance);
    }
}

CounterLayoutAppearer::~CounterLayoutAppearer() {

}

namespace NrvCounterLayoutAppearer {
    INIT_NERVE(CounterLayoutAppearerNrvHide);
    INIT_NERVE(CounterLayoutAppearerNrvAppear);
    INIT_NERVE(CounterLayoutAppearerNrvShow);
    INIT_NERVE(CounterLayoutAppearerNrvDisappear);

    void CounterLayoutAppearerNrvDisappear::execute(Spine *pSpine) const {
        CounterLayoutAppearer* layout = reinterpret_cast<CounterLayoutAppearer*>(pSpine->mExecutor);
        layout->exeDisappear();
    }

    void CounterLayoutAppearerNrvShow::execute(Spine *pSpine) const {
        CounterLayoutAppearer* layout = reinterpret_cast<CounterLayoutAppearer*>(pSpine->mExecutor);
        
        if (MR::isFirstStep(layout)) {
            layout->updateLayoutOffset(1.0f);
        }
    }

    void CounterLayoutAppearerNrvAppear::execute(Spine *pSpine) const {
        CounterLayoutAppearer* layout = reinterpret_cast<CounterLayoutAppearer*>(pSpine->mExecutor);
        layout->exeAppear();
    }

    void CounterLayoutAppearerNrvHide::execute(Spine *pSpine) const {
        
    }
};
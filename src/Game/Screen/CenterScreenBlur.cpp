#include "Game/LiveActor/Nerve.hpp"
#include "Game/Screen/CenterScreenBlur.hpp"
#include "Game/Screen/FullScreenBlur.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvCenterScreenBlur {
    NEW_NERVE(CenterScreenBlurNrvFadeIn, CenterScreenBlur, FadeIn);
    NEW_NERVE(CenterScreenBlurNrvKeep, CenterScreenBlur, Keep);
    NEW_NERVE(CenterScreenBlurNrvFadeOut, CenterScreenBlur, FadeOut);
}; // namespace NrvCenterScreenBlur

CenterScreenBlur::CenterScreenBlur()
    : LiveActor("画面中心ブラー"),
      _8C(0),
      _90(0),
      _94(0),
      _98(0.0f),
      _9C(0),
      _A0(0.0f) {
}

void CenterScreenBlur::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, 0x17, -1, -1, 0x2F);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    initNerve(&NrvCenterScreenBlur::CenterScreenBlurNrvFadeIn::sInstance);
    makeActorDead();
}

void CenterScreenBlur::appear() {
    LiveActor::appear();

    _A0 = 0.0f;

    setNerve(&NrvCenterScreenBlur::CenterScreenBlurNrvFadeIn::sInstance);
}

void CenterScreenBlur::draw() const {
    if (!MR::isDead(this)) {
        f32 a = _98 * _A0;
        u8  b = _9C * _A0;

        MR::drawFullScreenBlur(a, a, b, b);
    }
}

void CenterScreenBlur::start(s32 a1, f32 a2, u8 a3, s32 a4, s32 a5) {
    _8C = a1;
    _90 = a4;
    _94 = a5;
    _98 = a2;
    _9C = a3;

    appear();
}

void CenterScreenBlur::exeFadeIn() {
    _A0 = MR::calcNerveRate(this, _90);

    MR::setNerveAtStep(this, &NrvCenterScreenBlur::CenterScreenBlurNrvKeep::sInstance, _90);
}

void CenterScreenBlur::exeKeep() {
    if (MR::isFirstStep(this)) {
        _A0 = 1.0f;
    }

    if (MR::isGreaterEqualStep(this, _8C - (_90 + _94))) {
        setNerve(&NrvCenterScreenBlur::CenterScreenBlurNrvFadeOut::sInstance);
    }
}

void CenterScreenBlur::exeFadeOut() {
    _A0 = 1.0f - MR::calcNerveRate(this, _94);

    if (MR::isStep(this, _94)) {
        kill();
    }
}

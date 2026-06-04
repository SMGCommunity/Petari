#include "Game/Screen/CenterScreenBlur.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/FullScreenBlur.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvCenterScreenBlur {
    NEW_NERVE(CenterScreenBlurNrvFadeIn, CenterScreenBlur, FadeIn);
    NEW_NERVE(CenterScreenBlurNrvKeep, CenterScreenBlur, Keep);
    NEW_NERVE(CenterScreenBlurNrvFadeOut, CenterScreenBlur, FadeOut);
};  // namespace NrvCenterScreenBlur

CenterScreenBlur::CenterScreenBlur()
    : LiveActor("画面中心ブラー"), mTotalFrame(0), mFadeInFrame(0), mFadeOutFrame(0), mOffset(0.0f), mAlpha(0), _A0(0.0f) {
}

void CenterScreenBlur::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_ImageEffect, -1, -1, MR::DrawType_CenterScreenBlur);
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
    if (MR::isDead(this)) {
        return;
    }

    f32 a = mOffset * _A0;
    u8 b = mAlpha * _A0;

    MR::drawFullScreenBlur(a, a, b, b);
}

void CenterScreenBlur::start(s32 totalFrame, f32 offset, u8 alpha, s32 fadeInFrame, s32 fadeOutFrame) {
    mTotalFrame = totalFrame;
    mFadeInFrame = fadeInFrame;
    mFadeOutFrame = fadeOutFrame;
    mOffset = offset;
    mAlpha = alpha;

    appear();
}

void CenterScreenBlur::exeFadeIn() {
    _A0 = MR::calcNerveRate(this, mFadeInFrame);

    MR::setNerveAtStep(this, &NrvCenterScreenBlur::CenterScreenBlurNrvKeep::sInstance, mFadeInFrame);
}

void CenterScreenBlur::exeKeep() {
    if (MR::isFirstStep(this)) {
        _A0 = 1.0f;
    }

    if (MR::isGreaterEqualStep(this, mTotalFrame - (mFadeInFrame + mFadeOutFrame))) {
        setNerve(&NrvCenterScreenBlur::CenterScreenBlurNrvFadeOut::sInstance);
    }
}

void CenterScreenBlur::exeFadeOut() {
    _A0 = 1.0f - MR::calcNerveRate(this, mFadeOutFrame);

    if (MR::isStep(this, mFadeOutFrame)) {
        kill();
    }
}

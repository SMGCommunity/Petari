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

CenterScreenBlur::CenterScreenBlur() : LiveActor("画面中心ブラー"), mTime(), mFadeIn(), mFadeOut(), mOffset(), mAlpha(), mBlendRate() {
}

void CenterScreenBlur::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_ImageEffect, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_CenterScreenBlur);
    MR::invalidateClipping(this);
    MR::registerDemoSimpleCastAll(this);
    initNerve(GET_NERVE(CenterScreenBlur, CenterScreenBlurNrvFadeIn));
    makeActorDead();
}

void CenterScreenBlur::appear() {
    LiveActor::appear();

    mBlendRate = 0.0f;

    setNerve(GET_NERVE(CenterScreenBlur, CenterScreenBlurNrvFadeIn));
}

void CenterScreenBlur::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    f32 a = mOffset * mBlendRate;
    u8 b = mAlpha * mBlendRate;

    MR::drawFullScreenBlur(a, a, b, b);
}

void CenterScreenBlur::start(s32 time, f32 offset, u8 alpha, s32 fadeIn, s32 fadeOut) {
    mTime = time;
    mFadeIn = fadeIn;
    mFadeOut = fadeOut;
    mOffset = offset;
    mAlpha = alpha;

    appear();
}

void CenterScreenBlur::exeFadeIn() {
    mBlendRate = MR::calcNerveRate(this, mFadeIn);

    MR::setNerveAtStep(this, GET_NERVE(CenterScreenBlur, CenterScreenBlurNrvKeep), mFadeIn);
}

void CenterScreenBlur::exeKeep() {
    if (MR::isFirstStep(this)) {
        mBlendRate = 1.0f;
    }

    if (MR::isGreaterEqualStep(this, mTime - (mFadeIn + mFadeOut))) {
        setNerve(GET_NERVE(CenterScreenBlur, CenterScreenBlurNrvFadeOut));
    }
}

void CenterScreenBlur::exeFadeOut() {
    mBlendRate = 1.0f - MR::calcNerveRate(this, mFadeOut);

    if (MR::isStep(this, mFadeOut)) {
        kill();
    }
}

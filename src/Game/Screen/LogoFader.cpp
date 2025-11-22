#include "Game/Screen/LogoFader.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

#define RATE_MIN 0.0f
#define RATE_MAX 1.0f

namespace {
    NEW_NERVE(LogoFaderNrvBlank, LogoFader, Blank);
    NEW_NERVE(LogoFaderNrvDisplay, LogoFader, Display);
    NEW_NERVE(LogoFaderNrvFadeIn, LogoFader, FadeIn);
    NEW_NERVE(LogoFaderNrvFadeOut, LogoFader, FadeOut);
};  // namespace

LogoFader::LogoFader(const char* pName) : LayoutActor(pName, true), _20(true), mMaxStep(30), mRate(0.0f) {
    initNerve(&LogoFaderNrvDisplay::sInstance);
    kill();
}

void LogoFader::draw() const {
    if (mRate > RATE_MIN) {
        GXSetColorUpdate(GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

        GXColor fillColor = {0, 0, 0, mRate * 255.0f};
        MR::fillScreen(fillColor);
    }
}

void LogoFader::exeBlank() {
    if (MR::isFirstStep(this)) {
        mRate = RATE_MAX;
    }
}

void LogoFader::exeDisplay() {
    if (MR::isFirstStep(this)) {
        mRate = RATE_MIN;
    }
}

// FIXME: Conversion from integer to floating-point is not producing the correct instructions.
void LogoFader::exeFadeIn() {
    f32 step = getNerveStep();
    f32 maxStep = mMaxStep;

    mRate = RATE_MAX - (step / maxStep);

    if (mRate < RATE_MIN) {
        mRate = RATE_MIN;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(&LogoFaderNrvDisplay::sInstance);
    }
}

// FIXME: Conversion from integer to floating-point is not producing the correct instructions.
void LogoFader::exeFadeOut() {
    f32 step = getNerveStep();
    f32 maxStep = mMaxStep;

    mRate = step / maxStep;

    if (mRate > RATE_MAX) {
        mRate = RATE_MAX;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(&LogoFaderNrvBlank::sInstance);
    }
}

void LogoFader::setBlank() {
    setNerve(&LogoFaderNrvBlank::sInstance);
}

void LogoFader::startFadeIn() {
    setNerve(&LogoFaderNrvFadeIn::sInstance);
}

void LogoFader::startFadeOut() {
    setNerve(&LogoFaderNrvFadeOut::sInstance);
}

bool LogoFader::isFadeEnd() const {
    return isNerve(&LogoFaderNrvDisplay::sInstance) || isNerve(&LogoFaderNrvBlank::sInstance);
}

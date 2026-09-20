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

void LogoFader_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

LogoFader::LogoFader(const char* pName) : LayoutActor(pName, true), _20(true), mMaxStep(30), mRate(0.0f) {
    initNerve(GET_NERVE_ANON(LogoFaderNrvDisplay));
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

void LogoFader::exeFadeIn() {
    f32 step = getNerveStep();
    f32 maxStep = mMaxStep;

    mRate = RATE_MAX - (step / maxStep);

    if (mRate < RATE_MIN) {
        mRate = RATE_MIN;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(GET_NERVE_ANON(LogoFaderNrvDisplay));
    }
}

void LogoFader::exeFadeOut() {
    f32 step = getNerveStep();
    f32 maxStep = mMaxStep;

    mRate = step / maxStep;

    if (mRate > RATE_MAX) {
        mRate = RATE_MAX;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(GET_NERVE_ANON(LogoFaderNrvBlank));
    }
}

void LogoFader::setBlank() {
    setNerve(GET_NERVE_ANON(LogoFaderNrvBlank));
}

void LogoFader::startFadeIn() {
    setNerve(GET_NERVE_ANON(LogoFaderNrvFadeIn));
}

void LogoFader::startFadeOut() {
    setNerve(GET_NERVE_ANON(LogoFaderNrvFadeOut));
}

bool LogoFader::isFadeEnd() const {
    return isNerve(GET_NERVE_ANON(LogoFaderNrvDisplay)) || isNerve(GET_NERVE_ANON(LogoFaderNrvBlank));
}

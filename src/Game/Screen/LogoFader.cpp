#include "Game/Screen/LogoFader.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/LayoutUtil.hpp"

#define RATE_MIN 0.0f
#define RATE_MAX 1.0f

LogoFader::LogoFader(const char* pName) :
    LayoutActor(pName, true),
    _20(true),
    mMaxStep(30),
    mRate(0.0f)
{
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

#ifdef NON_MATCHING
// Conversion from integer to floating-point is not producing the correct instructions.
void LogoFader::exeFadeIn() {
    float step = getNerveStep();
    float maxStep = mMaxStep;

    mRate = RATE_MAX - (step / maxStep);

    if (mRate < RATE_MIN) {
        mRate = RATE_MIN;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(&LogoFaderNrvDisplay::sInstance);
    }
}
#endif

#ifdef NON_MATCHING
// Conversion from integer to floating-point is not producing the correct instructions.
void LogoFader::exeFadeOut() {
    float step = getNerveStep();
    float maxStep = mMaxStep;

    mRate = step / maxStep;

    if (mRate > RATE_MAX) {
        mRate = RATE_MAX;
    }

    if (MR::isStep(this, mMaxStep)) {
        setNerve(&LogoFaderNrvBlank::sInstance);
    }
}
#endif

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
    return isNerve(&LogoFaderNrvDisplay::sInstance)
        || isNerve(&LogoFaderNrvBlank::sInstance);
}

LogoFader::~LogoFader() {
    
}

namespace {
    INIT_NERVE(LogoFaderNrvBlank);
    INIT_NERVE(LogoFaderNrvDisplay);
    INIT_NERVE(LogoFaderNrvFadeIn);
    INIT_NERVE(LogoFaderNrvFadeOut);
};

#include "Game/Screen/WipeFade.hpp"
#include "Game/Util/LayoutUtil.hpp"
#include "Game/Util/DrawUtil.hpp"

WipeFade::WipeFade(const char* pName, const Color8& rFillColor)
    : WipeLayoutBase(pName),
      mIsWipeIn(false),
      mStepNum(30),
      mStep(30) {
    mFillColor = rFillColor;
}

void WipeFade::init(const JMapInfoIter& rIter) {
    kill();
}

void WipeFade::control() {
    mStep++;

    if (isOpen()) {
        kill();
    }
}

void WipeFade::draw() const {
    if (MR::isDead(this)) {
        return;
    }

    f32 stepRate = (f32)mStep / mStepNum;
    f32 progress = (stepRate < 0f) ? 0f : (stepRate > 1f) ? 1 : stepRate;
    if (mIsWipeIn) {
        progress = 1f - progress;
    }

    GXSetColorUpdate(GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);

    GXColor fillColor;
    fillColor.r = mFillColor.r;
    fillColor.g = mFillColor.g;
    fillColor.b = mFillColor.b;
    fillColor.a = 255f * progress;
    MR::fillScreen(fillColor);
}

void WipeFade::wipe(s32 step) {
    mIsWipeIn = !mIsWipeIn;

    if (step < 0) {
        mStepNum = 30;
    } else if (step == 0) {
        mStepNum = 1;
    } else {
        mStepNum = step;
    }

    mStep = 0;

    if (MR::isDead(this)) {
        appear();
    }
}

void WipeFade::forceClose() {
    mIsWipeIn = false;
    mStep = mStepNum;

    if (MR::isDead(this)) {
        appear();
    }
}

void WipeFade::forceOpen() {
    mIsWipeIn = true;
    mStep = mStepNum;

    kill();
}

bool WipeFade::isOpen() const {
    return mIsWipeIn && mStep >= mStepNum;
}

bool WipeFade::isClose() const {
    return !mIsWipeIn && mStep >= mStepNum;
}

bool WipeFade::isWipeIn() const {
    return mIsWipeIn && mStep < mStepNum;
}

bool WipeFade::isWipeOut() const {
    return !mIsWipeIn && mStep < mStepNum;
}

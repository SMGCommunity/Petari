#include "Game/AudioLib/AudEffector.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>

void AudEffector_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

JASDsp::FxlineConfig_ AudEffector::mFxLine[] = {
    {1, 0, 0, 0x7000, 1, 0x0000, 0, 64, {0, 0x1000, 0, 0, 0, 0, 0x3FFF, 0}},
    {1, 0, 0, 0x0000, 1, 0x7000, 0, 64, {0, 0x1000, 0, 0, 0, 0, 0x3FFF, 0}},
    {1, 0, 0, 0x7000, 1, 0x0000, 0, 64, {0, 0x1000, 0, 0, 0, 0, 0x3FFF, 0}},
    {1, 0, 0, 0x0000, 1, 0x7000, 0, 64, {0, 0x1000, 0, 0, 0, 0, 0x3FFF, 0}},
};

AudEffector::AudEffector() : mFxSend(), mCutoff(0x7F), mSpecialFx() {
    for (u8 i = 0; i < 4; i++) {
        mFxLines[i] = nullptr;
    }
}

void AudEffector::initFxLine(JKRHeap* pHeap) {
    for (u8 i = 0; i < 4; i++) {
        mFxLines[i] = new (pHeap, 0x20) s16[0x2800];
        JASDsp::setFXLine(i, mFxLines[i], &mFxLine[i]);
    }
}

void AudEffector::setFxSend(f32 fxSend) {
    if (fxSend < 0.0f) {
        fxSend = 0.0f;
    }
    if (fxSend > 1.0f) {
        fxSend = 1.0f;
    }
    mFxSend = fxSend;
}

void AudEffector::setCutoff(s32 cutoff) {
    if (cutoff < 0 || cutoff > 0x7F) {
        cutoff = 0x7F;
    }
    mCutoff = cutoff;
}

void AudEffector::setSpecialFx(s32 specialFx) {
    mSpecialFx = specialFx;
}

void AudEffector::initParams() {
    mCutoff = 0x7F;
    mFxSend = 0.0f;
    mSpecialFx = 0;
}

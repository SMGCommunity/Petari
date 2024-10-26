#include "Game/RhythmLib/AudBgmTempoAdjuster.hpp"
#include "Game/AudioLib/AudParams.hpp"

AudBgmTempoAdjuster::AudBgmTempoAdjuster() {
    mStream = nullptr;
    _4 = 1.0f;
    _8 = 0;
    _C = 0;
    _10 = 0;
    _14 = 0;
    _18 = 0.0f;
    init();
}

void AudBgmTempoAdjuster::init() {
    _1C = 0;
    _20 = 0;
    _24 = false;
    _25 = false;
}

void AudBgmTempoAdjuster::setStream(JAIStream *pStream) {
    BOOL inter = OSDisableInterrupts();
    init();
    mStream = pStream;
    OSRestoreInterrupts(inter);
}

void AudBgmTempoAdjuster::reject() {
    BOOL inter = OSDisableInterrupts();
    mStream = nullptr;
    init();
    OSRestoreInterrupts(inter);
}

// AudBgmTempoAdjuster::beatProc

f32 AudBgmTempoAdjuster::calcAdjustRate(f32 a1, f32 a2) {
    bool val = false;
    f32 v4 = ((a1 - 500.0f) / (_4 * ((60.0f * AudParams::streamSampleRate) / a2)));

    if (v4 < 0.0f) {
        val = true;
    }

    f32 v5 = (v4 * v4);

    if (v5 > 1.0f) {
        v5 = 1.0f;
    }

    if (val) {
        return (1.0f / (1.0f + v5));
    }

    return v5 + 1.0f;
}
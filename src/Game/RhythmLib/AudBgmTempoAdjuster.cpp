#include "Game/RhythmLib/AudBgmTempoAdjuster.hpp"
#include "Game/AudioLib/AudParams.hpp"
#include "JSystem/JAudio2/JAIStream.hpp"
#include "JSystem/JAudio2/JASCriticalSection.hpp"

AudBgmTempoAdjuster::AudBgmTempoAdjuster()
    : mStream(), mBeatMul(1.0f), mIntroBeats(), mLoopBeats(), mLoopStartSamples(), mLoopEndSamples(), mLoopSamples() {
    init();
}

void AudBgmTempoAdjuster::init() {
    mLoopCount = 0;
    mCurrentBeat = 0;
    mIsBeatNeedSync = false;
    mIsStreamNeedSync = false;
}

void AudBgmTempoAdjuster::setInfo(f32 beatMul, u32 introBeats, u32 loopBeats, u32 loopStartSamples, u32 loopEndSamples) {
    mBeatMul = beatMul;
    mIntroBeats = introBeats;
    mLoopBeats = loopBeats;
    mLoopStartSamples = loopStartSamples;
    mLoopEndSamples = loopEndSamples;
    mLoopSamples = loopEndSamples - loopStartSamples;
}

void AudBgmTempoAdjuster::setStream(JAIStream* pStream) {
    JASCriticalSection crit;
    init();
    mStream = pStream;
}

void AudBgmTempoAdjuster::reject() {
    JASCriticalSection crit;
    mStream = nullptr;
    init();
}

f32 AudBgmTempoAdjuster::beatProc(f32 tempo) {
    if (mStream == nullptr) {
        return 1.0f;
    }

    f32 currSample = mStream->inner_.aramStream._0B8;
    u32 loopCount = mStream->inner_.aramStream._0C4;

    bool newStreamLoop = false;
    if (loopCount != mLoopCount) {
        newStreamLoop = true;
        mLoopCount = loopCount;
    }

    bool newBeatLoop = false;
    if (mLoopBeats != 0 && mCurrentBeat >= mIntroBeats + mLoopBeats) {
        mCurrentBeat = mIntroBeats;
        newBeatLoop = true;
    }

    // (samples/beat) * mBeatMul * mCurrentBeat
    f32 beatSampleStart = (60.0f * AudParams::streamSampleRate / tempo) * mBeatMul * mCurrentBeat;

    if (mLoopSamples != 0.0f) {
        if (newBeatLoop && !newStreamLoop && !mIsStreamNeedSync) {
            currSample -= mLoopSamples;
        } else if (!newBeatLoop && newStreamLoop && !mIsBeatNeedSync) {
            currSample += mLoopSamples;
        }
    }

    f32 samples = currSample - beatSampleStart;  // how many samples out-of-sync
    f32 rate = calcAdjustRate(samples, tempo);

    if (newBeatLoop) {
        mIsBeatNeedSync = true;
    }
    if (newStreamLoop) {
        mIsStreamNeedSync = true;
    }

    if (mIsBeatNeedSync && mIsStreamNeedSync) {
        mIsBeatNeedSync = false;
        mIsStreamNeedSync = false;
    }

    mCurrentBeat++;

    return rate;
}

f32 AudBgmTempoAdjuster::calcAdjustRate(f32 syncSamples, f32 tempo) {
    f32 rate = (syncSamples - 500.0f) / ((60.0f * AudParams::streamSampleRate / tempo) * mBeatMul);

    bool early = false;
    if (rate < 0.0f) {
        early = true;
    }

    rate *= rate;

    if (rate > 1.0f) {
        rate = 1.0f;
    }

    if (!early) {
        rate += 1.0f;
        return rate;
    }

    return 1.0f / (1.0f + rate);
}

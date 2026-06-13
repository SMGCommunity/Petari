#pragma once

#include <revolution/types.h>

class JASWaveArc;

struct JASWaveInfo {
    JASWaveInfo() {
        mBaseKey = 60;
        _24 = &one;
    }

    void copy(const JASWaveInfo& other) {
        mFormat = other.mFormat;
        mBaseKey = other.mBaseKey;
        mSampleRate = other.mSampleRate;
        mAWStartOffs = other.mAWStartOffs;
        mAWLength = other.mAWLength;
        mLoopFlags = other.mLoopFlags;
        mSampleLoopStart = other.mSampleLoopStart;
        mSampleLoopEnd = other.mSampleLoopEnd;
        mSampleCount = other.mSampleCount;
        mpLastSample = other.mpLastSample;
        mpPenultSample = other.mpPenultSample;
        _24 = other._24;
    }

    /* 0x00 */ u8 mFormat;
    /* 0x01 */ u8 mBaseKey;
    /* 0x04 */ f32 mSampleRate;
    /* 0x08 */ int mAWStartOffs;
    /* 0x0C */ int mAWLength;
    /* 0x10 */ u32 mLoopFlags;
    /* 0x14 */ int mSampleLoopStart;
    /* 0x18 */ int mSampleLoopEnd;
    /* 0x1C */ int mSampleCount;
    /* 0x20 */ s16 mpLastSample;
    /* 0x22 */ s16 mpPenultSample;
    /* 0x24 */ const u32* _24;

    static u32 one;
};

class JASWaveHandle {
public:
    virtual ~JASWaveHandle() {
    }
    virtual const JASWaveInfo* getWaveInfo() const = 0;
    virtual int getWavePtr() const = 0;
};

class JASWaveBank {
public:
    virtual ~JASWaveBank() {
    }
    virtual JASWaveHandle* getWaveHandle(u32) const = 0;
    virtual JASWaveArc* getWaveArc(u32) = 0;
    virtual u32 getArcCount() const = 0;
};

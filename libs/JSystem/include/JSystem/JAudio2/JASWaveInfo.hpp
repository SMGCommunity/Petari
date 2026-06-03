#pragma once

#include <revolution/types.h>

class JASWaveArc;

struct JASWaveInfo {
    JASWaveInfo() {
        _1 = 0x3c;
        _20 = &one;
    }

    /* 0x00 */ u8 _0;
    /* 0x01 */ u8 _1;
    /* 0x02 */ u8 _2;
    /* 0x04 */ f32 _4;
    /* 0x08 */ int _8;
    /* 0x0C */ int _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ int _14;
    /* 0x18 */ int _18;
    /* 0x1C */ s16 _1C;
    /* 0x1E */ s16 _1E;
    /* 0x20 */ const u32* _20;

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

#pragma once

#include <revolution/types.h>

class JASWaveArc;

struct JASWaveInfo {
    JASWaveInfo() {
        _1 = 0x3c;
        _24 = &one;
    }

    void copy(const JASWaveInfo& other) {
        _0 = other._0;
        _1 = other._1;
        _4 = other._4;
        _8 = other._8;
        _C = other._C;
        _10 = other._10;
        _14 = other._14;
        _18 = other._18;
        _1C = other._1C;
        _20 = other._20;
        _22 = other._22;
        _24 = other._24;
    }

    /* 0x00 */ u8 _0;
    /* 0x01 */ u8 _1;
    /* 0x04 */ f32 _4;
    /* 0x08 */ int _8;
    /* 0x0C */ int _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ int _14;
    /* 0x18 */ int _18;
    /* 0x1C */ int _1C;
    /* 0x20 */ s16 _20;
    /* 0x22 */ s16 _22;
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

#pragma once

#include <revolution/types.h>

struct JAUAudibleParam {
    JAUAudibleParam() {
    }

    JAUAudibleParam(u16 a1, u16 a2) {
        mAudibleSw = a1;
        _f1 = a2;
    }

    JAUAudibleParam(u32 id) {
        raw = id;
    }

    operator u32() const {
        return raw;
    }

    u16 getAudibleSw() const {
        return mAudibleSw;
    }

    u8 get_BIT11() const {
        return mAudibleSw >> 11 & 1;
    }

    bool calcVolume() const {
        return mAudibleSw >> 10 & 1;
    }

    bool calcFxMix() const {
        return mAudibleSw >> 9 & 1;
    }

    bool get_BIT8() const {
        return mAudibleSw >> 8 & 1;
    }

    bool calcPan() const {
        return mAudibleSw >> 7 & 1;
    }

    bool calcDolby() const {
        return mAudibleSw >> 6 & 1;
    }

    bool calcDoppler() const {
        return (raw >> 28 & 0xF) != 0;
    }

    u32 getDoppler() const {
        return (u32)((*(u16*)&raw >> 12) & 0xf);
        // return raw >> 28 & 0xF;
    }

    f32 getDopplerPower() const {
        return (u32)((*(u8*)&raw >> 12) & 0xf) * (1.0f / 15.0f);
    }

    f32 getDopplerPower2() const {
        return (u32)((*(u8*)&raw >> 12) & 0xf) * (1.0f / 15.0f);
    }

    u32 getVolDistBit() const {
        return 1 << (mAudibleSw >> 4 & 3);
    }

    union {
        /* 0x0 */ struct {
            /* 0x0 */ u16 mAudibleSw;
            /* 0x2 */ u16 _f1;
        };
        /* 0x0 */ struct {
            /* 0x0 */ u8 b0_0 : 4;
            /* 0x0 */ u8 b0_4 : 1;
            /* 0x0 */ u8 b0_5 : 1;
            /* 0x0 */ u8 b0_6 : 1;
            /* 0x0 */ u8 b0_7 : 1;
            /* 0x1 */ u8 b1_0 : 1;
            /* 0x1 */ u8 b1_1 : 1;
            /* 0x1 */ u8 mVolDistSetting : 2;
            /* 0x1 */ u8 b1_4_7 : 4;
            /* 0x2 */ u8 b2;
            /* 0x3 */ u8 b3;
        };
        /* 0x0 */ u32 raw;
    };
};

#pragma once

#include <revolution.h>

namespace JMath {
    class TRandom_fast_ {
    public:
        TRandom_fast_() {};

        TRandom_fast_(u32);

        inline u32 rand() {
            return mSeed = 0x19660d * mSeed + 0x3c6ef35f;
            ;
        }

        inline float getRandF() {
            // !@bug UB: in C++ it's not legal to read from an union member other
            // than the last one that was written to.
            union {
                f32 f;
                u32 s;
            } out;
            out.s = (rand() >> 9) | 0x3f800000;
            return out.f - 1;
        }

        u32 mSeed;
    };

}  // namespace JMath

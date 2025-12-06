#pragma once

#include <revolution.h>

namespace JMath {
    class TRandom_fast_ {
    public:
        TRandom_fast_(u32);

        inline u32 rand() {
            return mSeed = 0x19660d * mSeed + 0x3c6ef35f;
            ;
        }

        u32 mSeed;
    };

}  // namespace JMath

#pragma once

#include <revolution/types.h>

namespace JMath {
    
    class TRandom_fast_ {
    public:
        TRandom_fast_(u32);

        inline u32 rand() {
            return mEntropy = 0x19660d * mEntropy + 0x3c6ef35f;;
        }

        u32 mEntropy;
    };
    
}
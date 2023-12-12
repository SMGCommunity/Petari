#include "JSystem/JMath/JMath.hpp"

// this is a different type but it works for now
f32 sLookupTable[1024];

#ifdef NON_MATCHING
// some small issues
f32 JMAAcosRadian(f32 val) {
    if (val >= 1.0f) {
        return 0.0f;
    }

    if (val <= -1.0f) {
        return 3.1415927f;
    }

    if (val >= 0.0f) {
        u32 idx = 1023.5f * -val;
        return 1.5707964f + sLookupTable[idx];
    }

    u32 idx = 1023.5f * val;
    return sLookupTable[idx] - 1.5707964f;
}
#endif
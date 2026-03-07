#pragma once

#include <cmath>
#include <revolution/types.h>


static f32 flt_80630CA0[0x408];

namespace JGeometry {
    template < typename T >
    class TUtil {
    public:
        static int epsilonEquals(T a1, T a2, T a3) {
            int ret = 0;

            a1 -= a2;

            if (-a3 <= a1 && a1 <= a3) {
                ret = 1;
            }

            return ret;
        }

        static T sqrt(T val) NO_INLINE {
            if (val <= 0.0f) {
                return 0.0f;
            }

            float outVal = __frsqrte(val);
            return (val * (0.5f * outVal * (3.0f - (val * (outVal * outVal)))));
        }

        static f32 asin(f32 val) NO_INLINE {
            if (val >= 1.0f) {
                return 1.5707964f;
            }

            if (val <= -1.0f) {
                return -1.5707964f;
            }

            if (val < 0.0f) {
                return -flt_80630CA0[(u32)(1023.5f * -val)];
            }

            return flt_80630CA0[(u32)(1023.5f * val)];
        }

        static f32 PI() NO_INLINE { return 3.1415927f; }

        static T clamp(T val, T min, T max) NO_INLINE {
            if (val < min) {
                return min;
            }

            if (val > max) {
                return max;
            }

            return val;
        }

        static T inv_sqrt(T val);

        static inline double epsilon() { return 32.0f * FLT_EPSILON; }
    };
};  // namespace JGeometry

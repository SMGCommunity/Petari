#pragma once

static f32 flt_80630CA0[0x408];

namespace JGeometry {
    template<typename T>
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

        static T sqrt(T val) {
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

        static T clamp(T, T, T);
        static T inv_sqrt(T);
    };
};
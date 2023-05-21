#pragma once

#include <revolution.h>

namespace nw4r {
    namespace math {
        namespace detail {
            struct SinCosSample {
                f32 sin_val;
                f32 cos_val;
                f32 sin_delta;
                f32 cos_delta;
            };

            extern const SinCosSample gSinCosTbl[257];
        };

        #define DEG_TO_FIDX(deg)  ((deg) * (256.f / 360.f))

        f32 SinFIdx(f32);
        f32 CosFIdx(f32);

        inline f32 SinDeg(f32 deg) {
            return SinFIdx(DEG_TO_FIDX(deg));
        }

        inline f32 CosDeg(f32 deg) {
            return CosFIdx(DEG_TO_FIDX(deg));
        }
    };
};
#pragma once

#include <nw4r/math/arithmetic.h>
#include <nw4r/math/constant.h>
#include <revolution.h>

#define NW4R_MATH_IDX_TO_FIDX(x) ((x) * (1.0f / 256.0f))
#define NW4R_MATH_DEG_TO_FIDX(x) ((x) * (256.0f / 360.0f))
#define NW4R_MATH_RAD_TO_FIDX(x) ((x) * (128.0f / NW4R_MATH_PI))
#define NW4R_MATH_DEG_TO_RAD(deg) ((deg) * 0.01745329252f)

namespace nw4r {
    namespace math {
        namespace detail {
            struct SinCosSample {
                f32 sin_val;
                f32 cos_val;
                f32 sin_delta;
                f32 cos_delta;
            };
        };  // namespace detail
        f32 SinFIdx(f32 fidx);

        inline f32 SinIdx(u16 idx) {
            return SinFIdx(NW4R_MATH_IDX_TO_FIDX(U16ToF32(idx)));
        }
        inline f32 SinDeg(f32 deg) {
            return SinFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
        }
        inline f32 SinRad(f32 rad) {
            return SinFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
        }

        f32 CosFIdx(f32 fidx);

        inline f32 CosDeg(f32 deg) {
            return CosFIdx(NW4R_MATH_DEG_TO_FIDX(deg));
        }
        inline f32 CosRad(f32 rad) {
            return CosFIdx(NW4R_MATH_RAD_TO_FIDX(rad));
        }
    };  // namespace math
};  // namespace nw4r

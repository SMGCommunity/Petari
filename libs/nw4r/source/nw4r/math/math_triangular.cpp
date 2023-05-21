#include "nw4r/math/triangular.h"
#include "nw4r/math/arithmetic.h"

namespace nw4r {
    namespace math {
        inline u16 F32ToU16(f32 x) {
            u16 rval;
            OSf32tou16(&x, &rval);
            return rval;
        }

        inline f32 U16ToF32(u16 x) {
            f32 rval;
            OSu16tof32(&x, &rval);
            return rval;
        }

        f32 SinFIdx(f32 fidx) {
            f32 abs_fidx;
            f32 val;
            u16 idx;
            f32 r;

            abs_fidx = FAbs(fidx);

            while (abs_fidx >= 65536.0f) {
                abs_fidx -= 65536.0f;
            }

            idx = F32ToU16(abs_fidx);
            r = abs_fidx - U16ToF32(idx);
            idx &= 0xFF;
            val = detail::gSinCosTbl[idx].sin_val + r * detail::gSinCosTbl[idx].sin_delta;
            return (fidx < 0.0f) ? -val : val;
        }

        f32 CosFIdx(f32 fidx) {
            u16 idx;
            f32 r;

            fidx = FAbs(fidx);

            while (fidx >= 65536.0f) {
                fidx -= 65536.0f;
            }

            idx = F32ToU16(fidx);
            r = fidx - U16ToF32(idx);
            idx &= 0xFF;
            return detail::gSinCosTbl[idx].cos_val + r * detail::gSinCosTbl[idx].cos_delta;
        }
    };
};
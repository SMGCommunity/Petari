#pragma once

#include <revolution/os.h>
#include <revolution/types.h>

namespace nw4r {
    namespace math {
        inline f32 U16ToF32(u16 x) {
            f32 rval;
            OSu16tof32(&x, &rval);
            return rval;
        }

        inline u16 F32ToU16(f32 x) {
            u16 rval;
            OSf32tou16(&x, &rval);
            return rval;
        }

        inline f32 FAbs(register f32 x) {
            register f32 ax;

            __asm {
                fabs ax, x
            }
            ;

            return ax;
        }

        inline f32 FSelect(register f32 cond, register f32 ifPos, register f32 ifNeg) {
            register f32 ret;
            asm { fsel   ret, cond, ifPos, ifNeg }
            return ret;
        }
    };  // namespace math
};  // namespace nw4r

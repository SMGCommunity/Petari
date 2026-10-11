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

#ifdef __MWERKS__
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
#else
        // 3DS port: portable equivalents of the PPC fabs / fsel instructions.
        inline f32 FAbs(f32 x) {
            return __builtin_fabsf(x);
        }

        // fsel: result = (cond >= 0.0) ? ifPos : ifNeg (NaN selects ifNeg).
        inline f32 FSelect(f32 cond, f32 ifPos, f32 ifNeg) {
            return cond >= 0.0f ? ifPos : ifNeg;
        }
#endif
    };  // namespace math
};  // namespace nw4r

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
    };  // namespace math
};  // namespace nw4r

#pragma once

#include <revolution.h>

namespace nw4r {
    namespace math {
        inline f32 FAbs(register f32 x) {
            register f32 ret;
            asm { fabs ret, x };
            return ret;
        }
    };
};
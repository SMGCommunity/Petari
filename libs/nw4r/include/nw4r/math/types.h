#pragma once

#include <revolution.h>

namespace nw4r {
    namespace math {
        struct _VEC2 {
            f32 x;
            f32 y;
        };

        struct _VEC3 {
            f32 x;
            f32 y;
            f32 z;
        };

        struct VEC2 : public _VEC2 {
        public:
            VEC2() {
                
            }

            VEC2(f32 _x, f32 _y) {
                x = _x;
                y = _y;
            }

            VEC2(const _VEC2 &v) {
                x = v.x;
                y = v.y;
            }
        };

        struct VEC3 : public _VEC3 {

        };
    };
};
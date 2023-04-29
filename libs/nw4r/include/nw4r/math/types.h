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

        struct _MTX33 {
            union {
                struct {
                    f32 _00, _01, _02;
                    f32 _10, _11, _12;
                    f32 _20, _21, _22;
                };

                f32 m[3][3];
                f32 a[9];
            };
        };

        struct _MTX34 {
            union {
                struct {
                    f32 _00, _01, _02, _03;
                    f32 _10, _11, _12, _13;
                    f32 _20, _21, _22, _23;
                };

                f32 m[3][4];
                f32 a[12];
                Mtx mtx;
            };
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

        struct MTX33 : public _MTX33 {

        };

        struct MTX34 : public _MTX34 {

        };
    };
};
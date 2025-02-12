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

        struct VEC2;
        struct VEC3;
        struct MTX34;

        struct VEC2 : public _VEC2 {

        };

        struct VEC3 : public _VEC3 {

        };

        struct MTX34 : public _MTX34 {

        };
    };
};

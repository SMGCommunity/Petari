#pragma once

#include <revolution.h>

namespace nw4r {
    namespace math {
        struct _VEC2
        {
            f32 x;
            f32 y;
        };

        struct _VEC3
        {
            f32 x;
            f32 y;
            f32 z;
        };

        struct _VEC4
        {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };

        struct _QUAT
        {
            f32 x;
            f32 y;
            f32 z;
            f32 w;
        };

        struct _QUAT16
        {
            s16 x;
            s16 y;
            s16 z;
            s16 w;
        };

        struct _MTX22
        {
            union
            {
                struct
                {
                    f32 _00, _01;
                    f32 _10, _11;
                };
                f32 m[2][2];
                f32 a[4];
            };
        };

        struct _MTX33
        {
            union
            {
                struct
                {
                    f32 _00, _01, _02;
                    f32 _10, _11, _12;
                    f32 _20, _21, _22;
                };
                f32 m[3][3];
                f32 a[9];
            };
        };

        struct _MTX34
        {
            union
            {
                struct
                {
                    f32 _00, _01, _02, _03;
                    f32 _10, _11, _12, _13;
                    f32 _20, _21, _22, _23;
                };
                f32 m[3][4];
                f32 a[12];
                Mtx mtx;
            };
        };

        struct _MTX44
        {
            union
            {
                struct
                {
                    f32 _00, _01, _02, _03;
                    f32 _10, _11, _12, _13;
                    f32 _20, _21, _22, _23;
                    f32 _30, _31, _32, _33;
                };
                f32 m[4][4];
                f32 a[16];
                Mtx44 mtx;
            };
        };

        struct VEC2 : public _VEC2 {

        };

        struct VEC3 : public _VEC3 {

        };

        struct VEC4 : public _VEC4 {
            
        };

        struct MTX33 : public _MTX33 {

        };

        struct MTX34 : public _MTX34 {

        };

        struct MTX44 : public _MTX44 {

        };


    };
};
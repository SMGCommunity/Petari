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
            typedef const f32 (*ConstMtxPtr)[4];

            MTX34() {

            }

            MTX34(const f32 *);
            MTX34(const Mtx &rhs);

            operator f32*() {
                return &_00;
            }

            operator const f32*() const { 
                return &_00;
            }

            operator MtxPtr() {
                return (MtxPtr)&_00;
            }

            operator ConstMtxPtr() const {
                return (ConstMtxPtr)&_00;
            }
        };

        inline MTX34* MTX34Copy(MTX34 *out, const MTX34 *p) {
            PSMTXCopy(*p, *out);
            return out;
        }

        inline MTX34* MTX34Mult(MTX34 *out, const MTX34 *p1, const MTX34 *p2) {
            PSMTXConcat(*p1, *p2, *out);
            return out;
        }

        inline MTX34::MTX34(const Mtx &rhs) {
            MTX34Copy(this, (MTX34*)&rhs);
        }

        VEC3* VECTransformNormal(VEC3 *, const MTX34 *, const VEC3 *);
    };
};
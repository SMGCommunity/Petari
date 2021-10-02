#pragma once

#include <revolution.h>

namespace JGeometry {
    template<typename T>
    class TVec2 {
    public:
        T x, y;
    };

    template<typename T>
    class TVec3 {
    public:
        TVec3() {

        }

        TVec3(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        TVec3(T val) {
            x = val;
            y = val;
            z = val;
        }

        template<typename T>
        void set(T _x, T _y, T _z);

        template<typename T>
        void set(const JGeometry::TVec3<T> &);

        T setLength(T);
        T squared() const;
        void zero();

        void sub(const JGeometry::TVec3<T> &);

        bool epsilonEquals(const JGeometry::TVec3<T> &, T) const;

        void scale(T);
        void scale(T, const JGeometry::TVec3<T> &);

        f32 dot(register const JGeometry::TVec3<f32> &rOther) const {
            register JGeometry::TVec3<f32>* this_vec = this;
            __asm {
                psq_l f2, 4(this_vec), 0, 0
                psq_l f1, 4(rOther), 0, 0
                psq_l f0, 0(this_vec), 0, 0
                ps_mul f2, f2, f1
                psq_l f1, 0(rOther), 0, 0
                ps_madd f1, f0, f1, f2
                ps_sum0 f1, f1, f2, f2
                blr
            };
        }

        void add(register const JGeometry::TVec3<f32> &rOther) {
            register JGeometry::TVec3<f32>* this_vec = this;
            __asm {
                psq_l f3, 0(this_vec), 0, 0
                psq_l f2, 0(rOther), 0, 0
                psq_l f1, 8(this_vec), 1, 0
                psq_l f0, 8(rOther), 1, 0
                ps_add f2, f3, f2
                ps_add f0, f1, f0
                psq_st f2, 0(this_vec), 0, 0
                psq_st f0, 8(this_vec), 1, 0
                blr
            }
        }

        const TVec3<T>& operator=(const TVec3<T> &);
        const TVec3<T>& operator-=(const TVec3<T> &);

        T x, y, z;
    };

    template<typename T>
    class TVec4 {
    public:
        template<typename T>
        void set(T _x, T _y, T _z, T _h) {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
        }

        T x, y, z, h;
    };
};

typedef JGeometry::TVec2<f32> TVec2f;
typedef JGeometry::TVec3<f32> TVec3f;
typedef JGeometry::TVec4<f32> TVec4f;
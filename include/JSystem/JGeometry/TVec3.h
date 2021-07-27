#pragma once

#include <revolution.h>

namespace JGeometry
{
    template<typename T>
    class TVec3
    {
    public:
        TVec3() { }
        TVec3(T);
        TVec3(const TVec3<T> &);
        TVec3(T a, T b, T c) : x(a), y(b), z(c) { }

        template<typename T>
        TVec3(T, T, T);
        
        f32 dot(const JGeometry::TVec3<T> &) const;
        void add(const JGeometry::TVec3<T> &) const;

        template<typename T>
        void set(const TVec3<T> &);
    
        template<typename T>
        void set(T, T, T);

        void set(TVec3<T> const &);
        void zero();

        bool epsilonEquals(const TVec3<T> &, T) const;

        void sub(const TVec3<T> &);

        void negate() const
        {
            const register TVec3<T>* vec = this;
            asm
            {
                psq_l f1, 0(vec), 0, 0
                lfs f0, 8(vec)
                ps_neg f1, f1
                fneg f0, f0
                psq_st f1, 0(vec), 0, 0
                stfs f0, 8(vec)
                blr
            }
        }

        T squared() const
        {
            const register TVec3<T>* vec = this;
            asm
            {
                psq_l f2, 0(vec), 0, 0
                lfs f0, 8(vec)
                ps_mul f2, f2, f2
                ps_madd f1, f0, f0, f2
                ps_sum0 f1, f1, f2, f2
                blr
            }
        }

        TVec3<T>& operator =(const register TVec3<T> &rhs)
        {
            const register TVec3<T>* lhs = this;
            asm
            {
                psq_l f1, 0(rhs), 0, 0
                lfs f0, 8(rhs)
                psq_st f1, 0(lhs), 0, 0
                stfs f0, 8(lhs)
            }

            return *this;
        };

        TVec3<T> operator +=(const TVec3<T> &);
        TVec3<T> operator -(const TVec3<T> &);
        TVec3<T> operator -=(const TVec3<T> &);
        bool operator==(const TVec3<T> &);

        void scale(T);

        T x; // _0
        T y; // _4
        T z; // _8
    };

    typedef JGeometry::TVec3<f32> TVec3f;
};

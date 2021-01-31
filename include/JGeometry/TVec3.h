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
        T squared() const;

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

        void scale(T);

        T x; // _0
        T y; // _4
        T z; // _8
    };
};
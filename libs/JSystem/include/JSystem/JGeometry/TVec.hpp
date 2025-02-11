#pragma once

#include "Inline.hpp"
#include <revolution.h>
// #include "math_types.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

namespace JGeometry {
    void negateInternal(const f32 *rSrc, f32 *rDest);

    template <typename T>
    struct TVec2 {
    public:
        /* Constructors */
        inline TVec2() {}

        template <typename A>
        TVec2(A _x, A _y)
        {
            x = _x;
            y = _y;
        }

        // inline
        TVec2(const TVec2<T> &rSrc); /*{
            x = rSrc.x;
            y = rSrc.y;
        }*/

        /* General operations */
        template <typename A>
        void set(const JGeometry::TVec2<A> &rSrc);

        template <typename A>
        void set(A _x, A _y);

        void setMin(const TVec2<T> &);
        void setMax(const TVec2<T> &);

        void sub(const TVec2<T> &rOther);
        T length() const;
        T squared() const;
        T squared(const TVec2<T> &) const;
        T dot(const TVec2<T> &rOther) const;
        T distance(const TVec2<T> &rOther) const;
        void zero();

        /* Operators */
        TVec2<T>& operator=(const TVec2<T> &rSrc);
        TVec2<T>& operator+(const TVec2<T> &rOther) const;
        TVec2<T>& operator-(const TVec2<T> &rOther) const;
        TVec2<T>& operator*(f32 scale) const;

        T x, y;
    };

    template <typename T>
    struct TVec3 {
        T x;
        T y;
        T z;

        inline TVec3(T val) {
            x = val;
            y = val;
            z = val;
        }

        void set(T x_, T y_, T z_) {
            x = x_;
            y = y_;
            z = z_;
        }

    };

    __attribute__((always_inline))
    inline void setTVec3f(const f32* a, f32* b) {
        #ifdef __MWERKS__
        const register f32* v_a = a;
        register f32* v_b = b;

        register f32 b_x;
        register f32 a_x;

        asm {
            psq_l a_x, 0(v_a), 0, 0
            lfs b_x, 8(v_a)
            psq_st a_x, 0(v_b), 0, 0
            stfs b_x, 8(v_b)
        };
        #endif
    }

    template<>
    struct TVec3<f32> : public Vec {
        inline TVec3(const Vec& vec) {
            setTVec3f(&vec.x, &x);
        }

        TVec3(const TVec3<f32> &vec) {
            setTVec3f(&vec.x, &x);
        }

        inline TVec3(f32 _x, f32 _y, f32 _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        TVec3(f32 val) {
            z = y = x = val;
        }

        inline TVec3() {}

        TVec3& operator=(const TVec3& b) NO_INLINE {
            setTVec3f(&b.x, &x);
            return *this;
        }

        TVec3& operator-=(const TVec3 &op) NO_INLINE {
            sub(op);
            return *this;
        }

        TVec3& operator+(const TVec3 &) const;
        TVec3& operator+=(const TVec3 &op);

        TVec3& operator*(f32) const;
        TVec3& operator*=(f32);

        TVec3& operator-() const;
        TVec3& operator-(const TVec3 &op) const;

        template <typename T>
        void set(const TVec3<f32>& rVec) {
            x = rVec.x;
            y = rVec.y;
            z = rVec.z;
        }

        template <typename T>
        void set(f32 _x, f32 _y, f32 _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        void set(f32 x_, f32 y_, f32 z_) {
            x = x_;
            y = y_;
            z = z_;
        }

        inline void set(f32 val) {
            z = val;
            y = val;
            z = val;
        }

        template<typename T>
        void setAll(f32);

        void sub(const TVec3<f32> &b) {
            JMathInlineVEC::PSVECSubtract(this, &b, this);
        }

        void sub(const TVec3 &, const TVec3 &);

        void setTrans(MtxPtr mtx) {
            set((*mtx)[3], (*mtx)[7], (*mtx)[11]);
        }

        inline void setPS(const TVec3<f32>& rVec) {
            setTVec3f(&rVec.x, &x);
        }

        void add(const TVec3<f32> &b) {
            JMathInlineVEC::PSVECAdd(this, &b, this);
        }

        void add(const TVec3 &, const TVec3 &);
        
        f32 squareMag() const {
            return JMathInlineVEC::PSVECSquareMag(this);
        }

        f32 dot(const TVec3 &) const;

        bool epsilonEquals(const TVec3<f32> &a1, f32 a2) const {
            bool ret = false;
            if (JGeometry::TUtil<f32>::epsilonEquals(x, a1.x, a2)) {
                if (JGeometry::TUtil<f32>::epsilonEquals(y, a1.y, a2)) {
                    if (JGeometry::TUtil<f32>::epsilonEquals(z, a1.z, a2)) {
                        ret = true;
                    }
                }
            }

            return ret;
        }

        void scale(f32);
        void scale(f32, const TVec3 &);
        void negate();
        f32 squared() const;
        f32 squared(const TVec3 &) const;
        void zero();
        bool isZero() const;
        void normalize(const TVec3 &);
        void setLength(f32);
        f32 setLength(const TVec3 &, f32);

        template <typename T>
        void cubic(const TVec3 &, const TVec3 &, const TVec3 &, const TVec3 &, f32);

        f32 angle(const TVec3 &) const;
    };

    template <typename T>
    struct TVec4 {
    public:
        /* Constructors */
        inline TVec4() {}

        template <typename A>
        TVec4(A _x, A _y, A _z, A _h)
        {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
        }

        /* General operations */
        template <typename A>
        void set(const JGeometry::TVec4<A> &);

        template <typename A>
        void set(A _x, A _y, A _z, A _h)
        {
            x = _x;
            y = _y;
            z = _z;
            h = _h;
        }

        void scale(T val);

        T x, y, z, h;
    };
};    // namespace JGeometry

typedef JGeometry::TVec2<f32> TVec2f;
typedef JGeometry::TVec3<f32> TVec3f;
typedef JGeometry::TVec3<s16> TVec3s;
typedef JGeometry::TVec4<f32> TVec4f;

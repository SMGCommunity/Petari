#pragma once

#include "Inline.hpp"
#include <revolution.h>
// #include "math_types.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

namespace JGeometry {
    void negateInternal(const f32 *rSrc, f32 *rDest);

    #ifdef __MWERKS__
    inline static void subInternal(register const f32 *vec1, register const f32 *vec2, register f32 *dst) 
    {
        register f32 xy2, z1, xy1, z2;
        __asm {
            psq_l xy1, 0(vec1), 0, 0
            psq_l xy2, 0(vec2), 0, 0
            psq_l z1, 8(vec1), 1, 0
            ps_sub xy1, xy1, xy2
            psq_st xy1, 0(dst), 0, 0
            psq_l z2, 8(vec2), 1, 0
            ps_sub z1, z2, z1
            psq_st z1, 8(dst), 1, 0
        }

    }
    #else
    static void subInternal(const f32 *vec1, const f32 *vec2, f32 *dst) ;
    #endif

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

        TVec3<T>(T _x, T _y, T _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        TVec3<T>(int x, int y, int Z);

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

        template<typename J>
        TVec3(J _x, J _y, J _z) {
            x = _x;
            y = _y;
            z = _z;
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

        inline TVec3(const Vec& vec) NO_INLINE {
            setTVec3f(&vec.x, &x);
        }

        TVec3(const TVec3<f32> &vec) {
            const register Vec* v_a = &vec;
            register Vec* v_b = this;
    
            register f32 b_x;
            register f32 a_x;
    
            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            };
        }

        // Can't be NO_INLINE (gets inlined in DiskGravity::DiskGravity())
        TVec3(f32 _x, f32 _y, f32 _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        template<typename T>
        TVec3(T, T, T);
        
        TVec3(f32 xz, f32 _y) {
            x = xz;
            y = _y;
            z = xz;
        }

        TVec3(f32 val) NO_INLINE {
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

        TVec3 operator+(const TVec3 &rVec) const {
            TVec3 ret(*this);
            JMathInlineVEC::PSVECAdd(&ret, &rVec, &ret);
            return ret;
        }
        TVec3& operator+=(const TVec3 &op);
        
        // Needs to be part of TVec to schedule instructions correctly in CubeGravity
        // Also, this seems like it should be merged with operator+(), but then how is
        // it that sometimes operator+() gets inlined several times without operator+= getting
        // inlined while other times, operator+() doesn't get inlined, and other times yet,
        // both operator+() and operator+=() both get inlined?
        TVec3 translate(const TVec3 &rSrc) const
        {
            TVec3 ret(*this);
            ret += rSrc;
            return ret;
        }

        TVec3 operator*(f32 scalar) const {
            TVec3 ret(*this);
            ret.x *= scalar;
            ret.y *= scalar;
            ret.z *= scalar;
            return ret;
        }
        
        TVec3& operator*=(f32);

        // Same reason to expect to merge as translate()
        TVec3 multiplyOperatorInline(f32 scalar) const {
            TVec3 ret(*this);
            ret *= scalar;
            return ret;
        }

        TVec3 operator-() const;

        // This should probably be merged with operator-(), but ParallelGravity doesn't inline
        // operator-() despite only referencing it once. So if we can match that, the two functions
        // can be merged.
        inline TVec3 negateInline() const {
            TVec3 ret;
            JMathInlineVEC::PSVECNegate(this, &ret);
            return ret;
        }
        
        TVec3 operator-(const TVec3 &op) const  {
            TVec3 ret(*this);
            JMathInlineVEC::PSVECSubtract(&ret, &op, &ret);
            return ret;
        }

        template <typename T>
        void set(const TVec3<f32>& rVec) NO_INLINE {
            x = rVec.x;
            y = rVec.y;
            z = rVec.z;
        }

        template <typename T>
        void set(T _x, T _y, T _z) NO_INLINE {
            x = _x;
            y = _y;
            z = _z;
        }

        template<typename T>
        void set(int x, int y, int z);

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

        #ifdef __MWERKS__
        inline void mulInternal(register const f32 *vec1, register const f32 *vec2, register f32 *dst) 
        {
    
            register f32 xy1, xy2, res;
            __asm {
                psq_l xy1, 0(vec1), 0, 0
                psq_l xy2, 0(vec2), 0, 0
                ps_mul res, xy1, xy2
                psq_st res, 0(dst), 0, 0
            }
            dst[2] = vec1[2] * vec2[2];
    
        }
        #else
        void mulInternal(const f32 *vec1, const f32 *vec2, f32 *dst) ;
        #endif


        void mult(const Vec &src1, const Vec &src2, Vec &dest) {
            mulInternal(&src1.x, &src2.x, &dest.x);
        }

        template<typename T>
        void setAll(f32);

        inline void mult(f32 val) {
            x *= val;
            y *= val;
            z *= val;
        }

        void sub(const TVec3<f32> &b) NO_INLINE {
            JMathInlineVEC::PSVECSubtract(this, &b, this);
        }

        void sub(const TVec3 &, const TVec3 &);

        void setTrans(MtxPtr mtx) {
            set((*mtx)[3], (*mtx)[7], (*mtx)[11]);
        }

        inline void setPS(const TVec3<f32>& rVec) {
            JGeometry::setTVec3f(&rVec.x, &this->x);
        }

        // Point gravity doesn't match if we use setPS
        inline void setPS2(const TVec3<f32>& rVec) {
            const register Vec* v_a = &rVec;
            register Vec* v_b = this;
    
            register f32 b_x;
            register f32 a_x;
    
            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            };
        }

        void add(const TVec3<f32> &b) NO_INLINE {
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

        inline void rejection(const TVec3 &rVec, const TVec3 &rNormal) {
            JMAVECScaleAdd(&rNormal, &rVec, this, -rNormal.dot(rVec));
        }

        void scale(f32);
        void scale(f32, const TVec3 &);
        void negate();
        f32 squared() const;
        f32 squared(const TVec3 &) const;
        void zero();
        bool isZero() const;
        f32 normalize(const TVec3 &);
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

typedef JGeometry::TVec2<s16> TVec2s;
typedef JGeometry::TVec2<f32> TVec2f;
typedef JGeometry::TVec3<f32> TVec3f;
typedef JGeometry::TVec3<s16> TVec3s;
typedef JGeometry::TVec4<f32> TVec4f;

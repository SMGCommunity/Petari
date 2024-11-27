#pragma once

#include "Inline.hpp"
#include <revolution.h>
// #include "math_types.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include <JSystem/JMath/JMath.hpp>

extern const Vec gZeroVec;

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
    public:
        /* Constructors */

        // This constructor must be associated with data so that construct_new_array
        // will use this (effectively empty) constructor. Otherwise, the compiler does not pass
        // any constructor to construct_new_array. See WireGravity::setPointListSize for more details.
        inline TVec3() {0.0f;}

        template <typename U>
        INLINE_FUNC_DECL(TVec3, U _x, U _y, U _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        //template <typename U>
        //TVec3(U _x, U _y, U _z);

        TVec3(T val)
        {
            x = val;
            y = val;
            z = val;
        }

        TVec3(T _xy, T _z)
        {
            x = _xy;
            y = _xy;
            z = _z;
        }

        template<typename A>
        TVec3(A _x, A _y , A _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        // inline
        TVec3(const TVec3<T> &rSrc); /*{
            setInline(rSrc);
        }*/

#ifdef __MWERKS__        
        inline TVec3<T>(const register Vec &rSrc) NO_INLINE
        {
            register TVec3<T> *dst = this;
            register const Vec *src = &rSrc;
            register f32 z;
            register f32 xy;
            __asm {
                psq_l xy, 0(src), 0, 0
                lfs z, 8(src)
                psq_st xy, 0(dst), 0, 0
                stfs z, 8(dst)
            };
        }
#else
        inline TVec3<T>(const Vec &rSrc) NO_INLINE;
#endif

        /* General operations */
        void set(const Vec &rSrc);

        template <typename A>
        void set(const JGeometry::TVec3<A> &rSrc) NO_INLINE
        {
            x = rSrc.x;
            y = rSrc.y;
            z = rSrc.z;
        }

        template <typename A>
        void set(A _x, A _y, A _z);

        void setZYX(T val)
        {
            z = val;
            y = val;
            x = val;
        }

        template <typename A>
        void setAll(A val);

        void add(const register TVec3<T> &rSrc) NO_INLINE
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
#if __MWERKS__
            __asm {
                psq_l f3, 0(this_vec), 0, 0
                psq_l f2, 0(rSrc), 0, 0
                psq_l     f1, 8(this_vec), 1, 0
                psq_l     f0, 8(rSrc), 1, 0
                ps_add f2, f3, f2
                ps_add f0, f1, f0
                psq_st f2, 0(this_vec), 0, 0
                psq_st f0, 8(this_vec), 0, 0
            };
#endif
        }

        void addXY(const register TVec3<T> &rSrc) NO_INLINE
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
#if __MWERKS__
            psq_l f3, 0(rSrc), 0, 0
            psq_st f0, 0(this_vec), 0, 0
#endif
        }

        void add(const TVec3<T> &, const TVec3<T> &);
        void sub(const TVec3<T> &);
        void sub(const TVec3<T> &, const TVec3<T> &);
        void mul(const TVec3<T> &);
        void mul(const TVec3<T> &, const TVec3<T> &);

        void scale(T scalar) NO_INLINE
        {
            x = x * scalar;
            y = y * scalar;
            z = z * scalar;
        }

        void scale(T, const TVec3<T> &);

        T setLength(T);
        T setLength(const TVec3<T>, T);
        T squared() const NO_INLINE
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
#if __MWERKS__
            __asm {
                psq_l f2, 0(this_vec), 0, 0
                lfs f0, 8(this_vec)
                ps_mul f2, f2, f2
                ps_madd f1, f0, f0, f2
                ps_sum0 f1, f1, f2, f2
                blr
            };
#endif
        }

        inline void scaleAdd(f32 val, const TVec3<f32> &a1, const TVec3<f32> &a2) {
            JMAVECScaleAdd(a1.toCVec(), a2.toCVec(), this->toVec(), val);
        }

        inline void cross(const TVec3<f32> &a1, const TVec3<f32> &a2) {
            PSVECCrossProduct(a1.toCVec(), a2.toCVec(), this->toVec());
        }
        
        inline T squaredInline() const
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
            register f32 _xy, _z;
#if __MWERKS__
            __asm {
                psq_l _xy, 0(this_vec), 0, 0
                lfs _z, 8(this_vec)
                ps_mul _xy, _xy, _xy
                ps_madd _z, _z, _z, _xy
                ps_sum0 _z, _z, _xy, _xy
            };
#endif
            return _z;
        }

        T squared(const TVec3<T> &) const;
        T angle(const TVec3<T> &rOther) const;
        

        inline void rejection(const TVec3<T> &rVec, const TVec3<T> &rNormal) 
        {
            JMAVECScaleAdd(rNormal.toCVec(), rVec.toCVec(), toVec(), -rNormal.dot(rVec));
        }

        TVec3<T>& subtract(const TVec3<T> &, const TVec3<T> &);
        void subtract(const TVec3<T> &);

        T dot(const register TVec3<T> &rOther) const NO_INLINE
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
#if __MWERKS__
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
#endif
        }

        void zero();

        void negate()
        {
            register const JGeometry::TVec3<f32> *this_vec = this;
#if __MWERKS__
            __asm {
                psq_l f1, 0(this_vec), 0, 0
                lfs f0, 8(this_vec)
                ps_neg f1, f1
                fneg f0, f0
                psq_st f1, 0(this_vec), 0, 0
                stfs f0, 8(this_vec)
            };
#endif
        }

        void negate(const TVec3<T> &rSrc);
        float normalize(const TVec3<T> &rSrc);

        inline TVec3<T> translate(const TVec3<T> &rSrc) const
        {
            TVec3<T> tmp(*this);
            tmp += rSrc;
            return tmp;
        }

        inline TVec3<T> translateOpposite(const TVec3<T> &rSrc) const
        {
            TVec3<T> tmp(*this);
            tmp -= rSrc;
            return tmp;
        }

        template <typename S>
        void cubic(const TVec3<T> &, const TVec3<T> &, const TVec3<T> &, const TVec3<T> &, T);

        bool isZero() const;
        bool epsilonEquals(const TVec3<T> &a1, T a2) const NO_INLINE
        {
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

        TVec3<T>& operator=(const TVec3<T> &rhs);

        /*TVec3<T>& operator=(const TVec3<T> &rhs) {
            TVec3<T> f = *this;
            JGeometry::setTVec3f((const f32*)rhs, (f32*)f);
            return f;
        }*/

        TVec3<f32>& operator*=(f32 scalar) {
            scale(scalar);
            return *this;
        }

        TVec3<T> operator*(T scalar) const NO_INLINE
        {
            TVec3<T> f = *this;
            f.scale(scalar);
            return f;
        }

        TVec3<T> operator-(const TVec3<T> &rhs) const NO_INLINE
        {
            TVec3<T> vec = *this;
            JMathInlineVEC::PSVECSubtract(vec.toCVec(), rhs.toCVec(), vec.toVec());
            return vec;
        }

        TVec3<T>& operator-=(const TVec3<T> &rhs) NO_INLINE
        {
            JMathInlineVEC::PSVECSubtract(toCVec(), rhs.toCVec(), toVec());
            return *this;
        }

        TVec3<T> operator+(const TVec3<T> &rhs) const NO_INLINE
        {
            TVec3<T> vec = *this;
            JMathInlineVEC::PSVECAdd(vec.toCVec(), rhs.toCVec(), vec.toVec());
            return vec;
        }

        TVec3<T>& operator+=(const TVec3<T> &rhs) NO_INLINE
        {
            JMathInlineVEC::PSVECAdd(toCVec(), rhs.toCVec(), toVec());
            return *this;
        }

        const TVec3<T> operator-() const NO_INLINE
        {
            register f32 z, xy;
#if __MWERKS__
            __asm {
                psq_l xy, 0(r4), 0, 0
                lfs z, 8(r4)
                ps_neg xy, xy
                fneg z, z
                psq_st xy, 0(r3), 0, 0
                stfs z, 8(r3)
                blr
            };
#endif
        }

        TVec3<T> operator%(T scalar) const
        {
            TVec3<T> f = *this;
            f.scale(scalar);
            return f;
        }

        inline TVec3<T> multInline2(T scalar) const
        {
            TVec3<T> f(*this);
            f.scaleInline(scalar);
            return f;
        }

        TVec3<T> operator/(T) const;

        bool operator==(const TVec3<T> &);

        /* Helper inline functions */
        inline void setInline(const TVec3<T> &src)
        {
            x = src.x;
            y = src.y;
            z = src.z;
        }

        inline void setInline(T _x, T _y, T _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        inline void setInline(T val)
        {
            x = val;
            y = val;
            z = val;
        }

        inline Vec *toVec() { return (Vec *)this; }

        inline const Vec *toCVec() const { return (const Vec *)this; }

        inline void setInlinePS(register const TVec3<T> &src)
        {
            register TVec3<T> *dst = this;
            register f32 xy;
            register f32 z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(src), 0, 0
                lfs z, 8(src)
                psq_st xy, 0(dst), 0, 0
                stfs z, 8(dst)
            };
#endif
        }

        inline void setInlinePS_2(register const TVec3<T> &src)
        {
            register TVec3<T> *dst = this;
            register f32 z;
            register f32 xy;
#if __MWERKS__
            __asm {
                psq_l xy, 0(src), 0, 0
                lfs z, 8(src)
                psq_st xy, 0(dst), 0, 0
                stfs z, 8(dst)
            };
#endif
        }

        inline void setInlineXYPS(register const TVec3<T> &src)
        {
            register TVec3<T> *dst = this;
            register f32 xy, scalar, _z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(src), 0, 0
                lfs _z, 8(src)
                psq_st xy, 0(dst), 0, 0
            };
#endif
            scalar = 500.0f;

            f32 __z = _z;
            f32 _y = x;
            f32 _x = y;
            __z = __z * scalar;
            _y = _y * scalar;
            _x = _x * scalar;
            z = __z;
            y = _x;
            x = _y;
        }

        inline void setZero()
        {
            register TVec3<T> *dst = this;
            register const Vec *src = &gZeroVec;
            register f32 z;
            register f32 xy;
#if __MWERKS__
            __asm {
                psq_l xy, 0(src), 0, 0
                lfs z, 8(src)
                psq_st xy, 0(dst), 0, 0
                stfs z, 8(dst)
            };
#endif
        }

        inline void scaleInline(T scale)
        {
            x *= scale;
            y *= scale;
            z *= scale;
        }

        inline void addInline(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            register f32 aXY, bXY, aZ, bZ;
#if __MWERKS__
            __asm {
                psq_l     aXY, 0(dst), 0, 0
                psq_l     bXY, 0(rOther), 0, 0
                psq_l     aZ, 8(dst), 1, 0
                psq_l     bZ, 8(rOther), 1, 0
                ps_add    bXY, aXY, bXY
                ps_add    bZ, aZ, bZ
                psq_st    bXY, 0(dst), 0, 0
                psq_st    bZ, 8(dst), 1, 0
            };
#endif
        }

        inline void addInline2(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            register f32 _2, _1, _0;

            __asm {
                psq_l     _0, 0(rOther), 0, 0
                psq_l     _2, 0(dst), 0, 0
                psq_l     _1, 8(dst), 1, 0
                ps_add    _0, _2, _0
                psq_st    _0, 0(dst), 0, 0
                psq_l     _0, 8(rOther), 1, 0
                ps_add    _0, _1, _0
                psq_st    _0, 8(dst), 1, 0
            };
        }

        inline void addInline3(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            // register f32 _2, _1, _0;
            register f32 dstXY, dstZ, srcXY, srcZ, totalZ;
#if __MWERKS__
            __asm {
                psq_l     dstXY, 0(dst), 0, 0
                psq_l     srcXY, 0(rOther), 0, 0
                psq_l     dstZ, 8(dst), 1, 0
                ps_add    dstXY, dstXY, srcXY
                psq_l     srcZ, 8(rOther), 1, 0
                ps_add    totalZ, dstZ, srcZ
                psq_st    dstXY, 0(dst), 0, 0
                psq_st    totalZ, 8(dst), 1, 0
            };
#endif
        }

        inline void addInline_4(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            register f32 bZ, bXY, aZ, aXY;
#if __MWERKS__
            __asm {
                psq_l     bXY, 0(rOther), 0, 0
                psq_l     bZ, 0(rOther), 1, 0
                psq_l     aXY, 0(dst), 0, 0
                psq_l     aZ, 8(dst), 1, 0
                ps_add    aXY, aXY, bXY
                ps_add    aZ, aZ, bZ
                psq_st    aXY, 0(dst), 0, 0
                psq_st    aZ, 8(dst), 1, 0
            };
#endif
        }

        inline void addInline_5(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            register f32 bZ, bXY, aZ, aXY;
#if __MWERKS__
            __asm {
                psq_l     bXY, 0(rOther), 0, 0
                psq_l     aXY, 0(dst), 0, 0
                psq_l     bZ, 0(rOther), 1, 0
                ps_add    bXY, bXY, aXY
                psq_l     aZ, 8(dst), 1, 0
                ps_add    aXY, bZ, aZ
                psq_st    bXY, 0(dst), 0, 0
                psq_st    aXY, 8(dst), 1, 0
            };
#endif
        }

        inline void addInline6(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            // register f32 _2, _1, _0;
            register f32 totalZ, dstZ, dstXY, srcZ, srcXY;

#if __MWERKS__
            __asm volatile {
                psq_l dstXY, 0(dst), 0, 0
                psq_l srcXY, 0(rOther), 0, 0
                psq_l dstZ, 8(dst), 1, 0
                ps_add dstXY, dstXY, srcXY
                psq_l srcZ, 8(rOther), 1, 0
                ps_add totalZ, dstZ, srcZ
                psq_st dstXY, 0(dst), 0, 0
                psq_st totalZ, 8(dst), 1, 0
            };
#endif
        }

        inline void addInline_7(register const TVec3<T> &rOther)
        {
            register TVec3<T> *dst = this;
            register f32 sumZ, bZ, aZ, bXY, aXY;
#if __MWERKS__
            __asm {
                psq_l     bXY, 0(rOther), 0, 0
                psq_l     aXY, 0(dst), 0, 0
                psq_l     aZ, 8(dst), 1, 0
                ps_add    aXY, aXY, bXY
                psq_l     bZ, 8(rOther), 1, 0
                ps_add    sumZ, aZ, bZ
                psq_st    aXY, 0(dst), 0, 0
                psq_st    sumZ, 8(dst), 1, 0
            };
#endif
        }

        inline TVec3<T> _madd(const TVec3<T> &v) const {
            TVec3<T> ret(*this);
            ret.addInline_7(v);
            return ret;
        }

        inline TVec3<T> madd(f32 scale, const TVec3<T> &v) const {
            return _madd(v * scale);
        }

        inline void subInline(const TVec3<T> &rA, const TVec3<T> &rB)
        {
            register TVec3<T> *dst = this;
            register const TVec3<T> *a = &rA;
            register const TVec3<T> *b = &rB;
            register f32 aXY, bZ, aZ, bXY;
            z = rA.z;
#if __MWERKS__
            __asm {
                psq_l     aXY, 0(a), 0, 0
                psq_l     bXY, 0(b), 0, 0
                psq_l     aZ, 8(dst), 1, 0
                psq_l     bZ, 8(b), 1, 0
                ps_sub    bXY, aXY, bXY
                ps_sub    aZ, aZ, bZ
                psq_st    bXY, 0(dst), 0, 0
                psq_st    aZ, 8(dst), 1, 0
            };
#endif
        }

        inline void subInline2(const TVec3<T> &rA, const TVec3<T> &rB)
        {
            register TVec3<T> *dst = this;
            register const TVec3<T> *a = &rA;
            register const TVec3<T> *b = &rB;
            register f32 bXY, aXY, bZ, aZ;
#if __MWERKS__
            __asm {
                psq_l     aXY, 0(a), 0, 0
                psq_l     bXY, 0(b), 0, 0
                ps_sub    aXY, aXY, bXY
                psq_st    aXY, 0(dst), 0, 0
                psq_l     aZ, 8(a), 1, 0
                psq_l     bZ, 8(b), 1, 0
                ps_sub    aZ, aZ, bZ
                psq_st    aZ, 8(dst), 1, 0
            };
#endif
        }

        inline void subInline3(const TVec3<T> &rA, const TVec3<T> &rB)
        {
            register TVec3<T> *dst = this;
            register const TVec3<T> *a = &rA;
            register const TVec3<T> *b = &rB;
            register f32 bZ, bXY, aXY, aZ;
#if __MWERKS__
            __asm {
                psq_l     aXY, 0(a), 0, 0
                psq_l     bXY, 0(b), 0, 0
                psq_l     bZ, 8(b), 1, 0
                ps_sub    aXY, aXY, bXY
                psq_st    aXY, 0(dst), 0, 0
                psq_l     aZ, 8(a), 1, 0
                ps_sub    aZ, aZ, bZ
                psq_st    aZ, 8(dst), 1, 0
            };
#endif
        }

        inline void subInline(const TVec3<T> &rA)
        {
            register TVec3<T> *this_vec = this;
            register const TVec3<T> *a = &rA;
            register f32 bZ, aZ, aXY, bXY;
#if __MWERKS__
            __asm {
                psq_l     aXY, 0(a), 0, 0
                psq_l     bXY, 0(this_vec), 0, 0
                ps_sub    bXY, bXY, aXY
                psq_st    bXY, 0(this_vec), 0, 0
                psq_l     aZ, 8(a), 1, 0
                psq_l     bZ, 8(this_vec), 1, 0
                ps_sub    aZ, bZ, aZ
                psq_st    aZ, 8(this_vec), 1, 0
            };
#endif
        }

        inline void subInline4(register const TVec3<T> &rVec)
        {
            register const TVec3<T> *this_vec = this;
            //register const TVec3<T> *a = &rVec;
            register f32 z_2, z_1, xy_2, xy_1, z_ret, xy_ret;
#if __MWERKS__
            __asm {
                psq_l xy_2, 0(rVec), 0, 0
                psq_l xy_1, 0(this_vec), 0, 0
                psq_l z_1, 8(this_vec), 1, 0
                ps_sub xy_ret, xy_1, xy_2
                psq_l z_2, 8(rVec), 1, 0
                ps_sub z_ret, z_1, z_2
                psq_st xy_ret, 0(this_vec), 0, 0
                psq_st z_ret, 8(this_vec), 1, 0
            };
#endif
        }

        inline const TVec3<T> negateInline() const
        {
            TVec3<T> ret;
            register const TVec3<T> *src = this;
            register TVec3<T> *dest = &ret;
            register f32 xy, z;
#if __MWERKS__
            __asm {
                psq_l     xy, 0(src), 0, 0
                ps_neg    xy, xy
                psq_st    xy, 0(dest), 0, 0
                lfs       z, 8(src)
                fneg      z, z
                stfs      z, 8(dest)
            };
#endif
            return ret;
        }

        inline const TVec3<T> negateInlineAndScale(T scalar) const
        {
            TVec3<T> ret;
            register const TVec3<T> *src = this;
            register TVec3<T> *dest = &ret;
            register f32 xy, z;
#if __MWERKS__
            __asm {
                psq_l     xy, 0(src), 0, 0
                ps_neg    xy, xy
                psq_st    xy, 0(dest), 0, 0
                lfs       z, 8(src)
                fneg      z, z
                stfs      z, 8(dest)
            };
#endif
            TVec3<f32> trueRet = TVec3f(ret);
            trueRet.scale(scalar);
            return trueRet;
        }

        inline void negateInline(register const TVec3<T> &rSrc)
        {
            register TVec3<T> *dst = this;
            register f32 xy;
            register f32 z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(rSrc), 0, 0
                lfs z, 8(rSrc)
                ps_neg xy, xy
                fneg z, z
                psq_st xy, 0(dst), 0, 0
                stfs z, 8(dst)
            };
#endif
        }

        inline void negateInline_2(register const TVec3<T> &rSrc)
        {
            register TVec3<T> *dst = this;
            register f32 xy;
            register f32 z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(rSrc), 0, 0
                ps_neg xy, xy
                psq_st xy, 0(dst), 0, 0
                lfs z, 8(rSrc)
                fneg z, z
                stfs z, 8(dst)
            };
#endif
        }

        inline TVec3<T> negateInline_2() const
        {
            TVec3<T> ret;
            ret.negateInline_2(*this);
            return ret;
        }

        inline void negateInlineAndStore(register const TVec3<T> &rSrc, register TVec3<T> &rDest)
        {
            register TVec3<T> *dst = this;
            register f32 xy;
            register f32 z_neg, x, y, z, z_single;
#if __MWERKS__
            __asm {
                psq_l xy, 0(rSrc), 0, 0
                lfs z, 8(rSrc)
                ps_neg xy, xy
                fneg z_neg, z
                psq_st xy, 0(dst), 0, 0
                frsp z_single, z_neg
                stfs z_neg, 8(dst)
                lfs x, 0(dst)
                lfs y, 4(dst)
                lfs z, 8(dst)
                stfs x, 0(rDest)
                stfs y, 4(rDest)
                stfs z_single, 8(rDest)
            };
#endif
        }

        inline void negateSelf()
        {
            register TVec3<T> *src = this;
            register TVec3<T> *dst = this;
            register f32 xy;
            register f32 z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(src), 0, 0
                ps_neg xy, xy
                psq_st xy, 0(dst), 0, 0
                lfs z, 8(src)
                fneg z, z
                stfs z, 8(dst)
            };
#endif
        }

        inline void setNegatedInline(register const TVec3<T> &rSrc)
        {
            register TVec3<T> *rDst = this;
            register f32 xy;
            register f32 z;
#if __MWERKS__
            __asm {
                psq_l xy, 0(rSrc), 0, 0
                ps_neg xy, xy
                psq_st xy, 0(rDst), 0, 0
                lfs z, 8(rSrc)
                fneg z, z
                stfs z, 8(rDst)
            };
#endif
        }

        inline void multAndSet(TVec3<T> *pDest, T scalar)
        {
            TVec3<T> mult_res = *this * scalar;
            pDest->x = mult_res.x;
            pDest->y = mult_res.y;
            pDest->z = mult_res.z;
        }

        inline void multPS(register TVec3<T> &rVec1, register TVec3<T> &rVec2)
        {
            register TVec3<T> *rDst = this;
            register f32 xy_1, xy_2;
            register f32 z_1, z_2;
#if __MWERKS__
            __asm {
                psq_l xy_1, 0(rVec1), 0, 0
                psq_l xy_2, 0(rVec2), 0, 0
                ps_mul xy_2, xy_2, xy_1
                psq_st xy_2, 0(rDst), 0, 0
                lfs z_1, 8(rVec2)
                lfs z_2, 8(rVec1)
                fmuls z_2, z_1, z_2
                stfs z_2, 8(rDst)
            };
#endif
        }

        inline f32 squareDistancePS(const register TVec3<T> &rVec1) const
        {
            register const TVec3<T> *this_vec = this;
            register f32 yz_1, x_1;
            register f32 yz_2, x_2;
            register f32 ret;
#if __MWERKS__
            __asm {
                psq_l yz_1, 4(this_vec), 0, 0
                psq_l yz_2, 4(rVec1), 0, 0
                psq_l x_1, 0(this_vec), 0, 0
                ps_sub yz_2, yz_1, yz_2
                psq_l x_2, 0(rVec1), 0, 0
                ps_sub x_2, x_1, x_2
                ps_mul yz_2, yz_2, yz_2
                ps_madd ret, x_2, x_2, yz_2
                ps_sum0 ret, ret, yz_2, yz_2
            };
#endif
            return ret;
        }

        T x, y, z;
    };

    /*template<>
    TVec3<f32>::TVec3(int _x, int _y, int _z) {
        x = _x;
        y = _y;
        x = _z;
    }*/

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

    void setTVec3f(register const f32 *src, register f32 *dst); /*{
        __asm {
            psq_l   f0, 0(src), 0, 0
            lfs     f1, 8(src)
            psq_st  f0, 0(dst), 0, 0
            stfs    f1, 8(dst)
        };
    }*/

};    // namespace JGeometry

typedef JGeometry::TVec2<f32> TVec2f;
typedef JGeometry::TVec3<f32> TVec3f;
typedef JGeometry::TVec3<s16> TVec3s;
typedef JGeometry::TVec4<f32> TVec4f;

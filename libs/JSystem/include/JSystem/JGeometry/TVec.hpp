#pragma once

#include "Inline.hpp"
#include <revolution.h>
// #include "math_types.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include "revolution/types.h"
#include <JSystem/JMath/JMath.hpp>

namespace JGeometry {
    void negateInternal(const f32* rSrc, f32* rDest);

#ifdef __MWERKS__
    inline static void subInternal(register const f32* vec1, register const f32* vec2, register f32* dst) {
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
    static void subInternal(const f32* vec1, const f32* vec2, f32* dst);
#endif

    template < typename T >
    struct TVec2 {
    public:
        /* Constructors */
        inline TVec2() {}

        template < typename A >
        TVec2(A _x, A _y) {
            x = _x;
            y = _y;
        }

        // inline
        inline TVec2(const TVec2< T >& rSrc) {
            x = rSrc.x;
            y = rSrc.y;
        }

        void add(const TVec2< T >& other) {
            x += other.x;
            y += other.y;
        }

        /* General operations */
        template < typename A >
        void set(const JGeometry::TVec2< A >& rSrc);

        void set(T v) { y = x = v; }

        template < typename U >
        void set(const U x, const U y) {
            this->x = x;
            this->y = y;
        }

        void setMin(const TVec2< f32 >& min) {
            if (x >= min.x)
                x = min.x;
            if (y >= min.y)
                y = min.y;
        }

        void setMax(const TVec2< f32 >& max) {
            if (x <= max.x)
                x = max.x;
            if (y <= max.y)
                y = max.y;
        }

        inline bool isAbove(const TVec2< T >& other) const { return (x >= other.x) && (y >= other.y) ? true : false; }

        void sub(const TVec2< T >& rOther);

        T length() const { return JGeometry::TUtil< T >::sqrt((x * x) + (y * y)); }

        T squared() const { return x * x + y * y; };
        T squared(const TVec2< T >& rOther) const;  //{ return (x - rOther.x) * (x - rOther.x) + (y - rOther.y) * (y - rOther.y); };
        T dot(const TVec2< T >& rOther) const;
        T distance(const TVec2< T >& rOther) const;
        void zero();

        /* Operators */
        TVec2< T >& operator=(const TVec2< T >& rSrc);
        TVec2< T >& operator+(const TVec2< T >& rOther) const;
        TVec2< T >& operator-(const TVec2< T >& rOther) const;
        TVec2< T >& operator*(f32 scale) const;

        T x, y;
    };

    template < typename T >
    struct TVec3 {
        T x;
        T y;
        T z;

        TVec3< T >(T _x, T _y, T _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        TVec3< T >(int x, int y, int Z);

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

        template < typename J >
        TVec3(J _x, J _y, J _z) {
            x = _x;
            y = _y;
            z = _z;
        }
    };

    template <>
    struct TVec3< s16 > {
        s16 x, y, z;

        TVec3() {}

        TVec3(s16 x, s16 y, s16 z) { set(x, y, z); }

        TVec3& operator=(const TVec3& b) {
            // Force copies to use lwz/lha
            *((s32*)this) = *((s32*)&b);
            z = b.z;
            return *this;
        }

        void set(s16 x_, s16 y_, s16 z_) {
            x = x_;
            y = y_;
            z = z_;
        }
    };

    __attribute__((always_inline)) inline void setTVec3f(const f32* a, f32* b) {
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
        }
        ;
#endif
    }

    template <>
    struct TVec3< f32 > : public Vec {
#ifdef __MWERKS__
        TVec3(const Vec& vec) {
            const register Vec* v_a = &vec;
            register Vec* v_b = this;

            register f32 b_x;
            register f32 a_x;

            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            }
            ;
        }
#else
        TVec3(const Vec& vec);
#endif
#ifdef __MWERKS__
        // Used inlined and non-inlined?
        TVec3(const TVec3< f32 >& vec) {
            const register Vec* v_a = &vec;
            register Vec* v_b = this;

            register f32 b_x;
            register f32 a_x;

            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            }
            ;
        }
#else
        TVec3(const TVec3< f32 >& vec);
#endif

        // Can't be NO_INLINE (gets inlined in DiskGravity::DiskGravity())
        template < typename T >
        TVec3(T _x, T _y, T _z) {
            x = _x;
            y = _y;
            z = _z;
        }

        TVec3(f32 xz, f32 _y) {
            x = xz;
            y = _y;
            z = xz;
        }

        TVec3(f32 val) NO_INLINE { z = y = x = val; }

        inline TVec3() {}

        operator Vec*() { return (Vec*)&x; }
        operator const Vec*() const { return (Vec*)&x; }

        TVec3& operator=(const TVec3& b) NO_INLINE {
            setTVec3f(&b.x, &x);
            return *this;
        }

        TVec3& operator-=(const TVec3& op) NO_INLINE {
            sub(op);
            return *this;
        }

        TVec3 operator+(const TVec3& op) const {
            TVec3 ret(*this);
            JMathInlineVEC::PSVECAdd(&ret, &op, &ret);
            return ret;
        }
        TVec3& operator+=(const TVec3& op);

        // Needs to be part of TVec to schedule instructions correctly in CubeGravity
        // Also, this seems like it should be merged with operator+(), but then how is
        // it that sometimes operator+() gets inlined several times without operator+= getting
        // inlined while other times, operator+() doesn't get inlined, and other times yet,
        // both operator+() and operator+=() both get inlined?
        TVec3 translate(const TVec3& op) const {
            TVec3 ret(*this);
            ret += op;
            return ret;
        }

        // needed to match TVec stack access order in many places
        TVec3 addOperatorInLine(const TVec3& op) const {
            TVec3 ret(*this);
            ret.add(op);
            return ret;
        }

        void addInLine(const TVec3& op) { JMathInlineVEC::PSVECAdd(this, &op, this); }

        TVec3 operator*(f32 scalar) const NO_INLINE {
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

        // appears to be needed in RingBeam to match stack in some places
        TVec3 scaleInline(f32 scalar) const {
            TVec3 ret(*this);
            ret.scale(scalar);
            return ret;
        }

        // needed in StarPieceFollowGroup???
        inline TVec3 multInLine(f32 val) {
            TVec3 ret(*this);
            ret.x *= val;
            ret.y *= val;
            ret.z *= val;
            return ret;
        }

        TVec3 operator-() const;

        bool operator==(const TVec3& rVec) const {
            return TUtil< f32 >::epsilonEquals(x, rVec.x, 0.0000038146973f) && TUtil< f32 >::epsilonEquals(y, rVec.y, 0.0000038146973f) &&
                   TUtil< f32 >::epsilonEquals(z, rVec.z, 0.0000038146973f);
        }

        void mul(const TVec3< f32 >& a, const TVec3< f32 >& b) { mulInternal(&a.x, &b.x, &this->x); }

        // This should probably be merged with operator-(), but ParallelGravity doesn't inline
        // operator-() despite only referencing it once. So if we can match that, the two functions
        // can be merged.
        inline TVec3 negateInline() const {
            TVec3 ret;
            JMathInlineVEC::PSVECNegate(this, &ret);
            return ret;
        }

        inline TVec3 negateOperatorInternal() const {
            TVec3 ret;
            JGeometry::negateInternal(&this->x, &ret.x);
            return ret;
        }

        inline void negateInternal() { JGeometry::negateInternal(&this->x, &this->x); }

        TVec3 operator-(const TVec3& op) const {
            TVec3 ret(*this);
            JMathInlineVEC::PSVECSubtract(&ret, &op, &ret);
            return ret;
        }

        TVec3 subOperatorInLine(const TVec3& op) const {
            TVec3 ret(*this);
            ret.sub(op);
            return ret;
        }

        template < typename T >
        void set(const TVec3< T >& rVec) {
            x = rVec.x;
            y = rVec.y;
            z = rVec.z;
        }

        template < typename T >
        void set(T _x, T _y, T _z) NO_INLINE {
            x = _x;
            y = _y;
            z = _z;
        }

        template < typename T >
        void set(int x, int y, int z);

        void set(f32 x_, f32 y_, f32 z_) {
            x = x_;
            y = y_;
            z = z_;
        }

        inline void set(f32 val) {
            x = val;
            y = val;
            z = val;
        }

#ifdef __MWERKS__
        inline void mulInternal(register const f32* vec1, register const f32* vec2, register f32* dst) {
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
        void mulInternal(const f32* vec1, const f32* vec2, f32* dst);
#endif

        void mult(const Vec& src1, const Vec& src2, Vec& dest) { mulInternal(&src1.x, &src2.x, &dest.x); }

        template < typename T >
        void setAll(f32);

        inline void mult(f32 val) {
            x *= val;
            y *= val;
            z *= val;
        }

        void sub(const TVec3< f32 >& b) NO_INLINE { JMathInlineVEC::PSVECSubtract(this, &b, this); }

        void sub(const TVec3&, const TVec3&);

        // Required for multiple objects to match?
        inline void multPS(TVec3< f32 >& a, TVec3< f32 >& b) { mulInternal(&b.x, &a.x, &this->x); }

        void setTrans(MtxPtr mtx) { set< f32 >((*mtx)[3], (*mtx)[7], (*mtx)[11]); }

        inline void setPS(const TVec3< f32 >& rSrc) { JGeometry::setTVec3f(&rSrc.x, &x); }

#ifdef __MWERKS__
        // Point gravity doesn't match if we use setPS
        inline void setPS2(const TVec3< f32 >& rSrc) {
            const register Vec* v_a = &rSrc;
            register Vec* v_b = this;

            register f32 b_x;
            register f32 a_x;

            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            }
            ;
        }

        inline void setPSZeroVec() {
            const register Vec* v_a = &gZeroVec;
            register Vec* v_b = this;

            register f32 b_x;
            register f32 a_x;

            asm {
                psq_l a_x, 0(v_a), 0, 0
                lfs b_x, 8(v_a)
                psq_st a_x, 0(v_b), 0, 0
                stfs b_x, 8(v_b)
            }
            ;
        }
#else
        void setPS2(const TVec3< f32 >& rSrc);
        void setPSZeroVec();
#endif

        void add(const TVec3< f32 >& b) NO_INLINE { JMathInlineVEC::PSVECAdd(this, &b, this); }
        inline void addInline(const TVec3< f32 >& b) { JMathInlineVEC::PSVECAdd(this, &b, this); }

        void add(const TVec3&, const TVec3&);

        f32 dot(const TVec3&) const;

        bool epsilonEquals(const TVec3< f32 >& a1, f32 a2) const {
            bool ret = false;
            if (JGeometry::TUtil< f32 >::epsilonEquals(x, a1.x, a2)) {
                if (JGeometry::TUtil< f32 >::epsilonEquals(y, a1.y, a2)) {
                    if (JGeometry::TUtil< f32 >::epsilonEquals(z, a1.z, a2)) {
                        ret = true;
                    }
                }
            }

            return ret;
        }

        void scaleAdd(__REGISTER f32 sc, const TVec3< f32 >& a, const TVec3< f32 >& b) { JMAVECScaleAdd(&a, &b, this, sc); }

        inline void scaleAdd(const TVec3& scaleVec, const TVec3& addVec, f32 scale) { JMAVECScaleAdd(&scaleVec, &addVec, this, scale); }

        inline void rejection(const TVec3& rVec, const TVec3& rNormal) { JMAVECScaleAdd(&rNormal, &rVec, this, -rNormal.dot(rVec)); }
        inline void rejection(const TVec3& rNormal) {
            const TVec3& norm = rNormal;
            JMAVECScaleAdd(&norm, this, this, -norm.dot(*this));
        }

        inline void invert() {
            this->x *= -1.0f;
            this->y *= -1.0f;
            this->z *= -1.0f;
        }

        inline void invertInternal() { JMathInlineVEC::PSVECNegate(this, this); }

        void scale(f32 scale);
        void scale(f32, const TVec3&);
        void negate();

        f32 normalize() {
            f32 sq = squared();
            if (sq <= TUtil< f32 >::epsilon()) {
                return 0.0f;
            }
            f32 inv_norm = TUtil< f32 >::inv_sqrt(sq);
            scale(inv_norm);
            return inv_norm * sq;
        }

        inline void mul(const TVec3< f32 >& a) { mul(*this, a); }

        void cross(const TVec3< f32 >& a, const TVec3< f32 >& b) { PSVECCrossProduct(a, b, *this); }

        f32 squared() const { return JMathInlineVEC::PSVECSquareMag(this); };

#ifdef __MWERKS__
        // this theoretically should just forward JMathInlineVEC::PSVECSquareDistance,
        // however using the exact same asm causes mismatches. Keeping it here instead
        // keeps the exact matches, and even allows for more use cases that match.
        f32 squared(const TVec3& rB) const {
            const register Vec* b = &rB;
            const register Vec* a = this;

            register f32 sqdist, dxy, dyz;
            register f32 v1xy, v0xy, v1yz, v0yz;

            asm
            {
                psq_l    v0yz, 4(a), 0, 0 
                psq_l    v1yz, 4(b), 0, 0
                ps_sub   dyz, v0yz, v1yz
            
                psq_l    v0xy, 0(a), 0, 0
                psq_l    v1xy, 0(b), 0, 0
                ps_sub   dxy, v0xy, v1xy
                ps_mul   dyz, dyz, dyz
            
                ps_madd  sqdist, dxy, dxy, dyz
                ps_sum0  sqdist, sqdist, dyz, dyz
            }

            return sqdist;
        };
#else
        f32 squared(const TVec3& rB) const;
#endif

        void zero();

        bool isZero() const { return squared() <= 0.0000038146973f; }

        f32 normalize(const TVec3& rSrc) {
            x = rSrc.x;
            y = rSrc.y;
            z = rSrc.z;
            float magnitude = PSVECMag(this);
            PSVECNormalize(this, this);
            return magnitude;
        }

        f32 setLength(f32 newlength) {
            f32 oldlength = squared();
            if (oldlength <= 0.0000038146973f) {
                return 0.0f;
            }
            f32 lengthinv = JGeometry::TUtil< f32 >::inv_sqrt(oldlength);
            scale(lengthinv * newlength);
            return lengthinv * oldlength;
        };

        f32 setLength2(f32 newlength) {
            f32 oldlength = squared();
            if (oldlength <= 0.0000038146973f) {
                return 0.0f;
            }
            f32 lengthinv = JGeometry::TUtil< f32 >::inv_sqrt(oldlength);
            x *= lengthinv * newlength;
            y *= lengthinv * newlength;
            z *= lengthinv * newlength;
            return lengthinv * oldlength;
        };

        f32 setLength(const TVec3&, f32);

        f32 length() const { return PSVECMag(this); }

        template < typename T >
        void cubic(const TVec3&, const TVec3&, const TVec3&, const TVec3&, f32);

        f32 angle(const TVec3&) const;

        inline TVec3 cross(const TVec3& b) {
            TVec3 ret;
            PSVECCrossProduct(this, &b, &ret);
            return ret;
        }
    };

    template < typename T >
    class TVec4 : public Quaternion {
    public:
        /* Constructors */
        inline TVec4() {}

        template < typename A >
        TVec4(A _x, A _y, A _z, A _h) {
            x = _x;
            y = _y;
            z = _z;
            w = _h;
        }

        /* General operations */
        template < typename A >
        void set(const JGeometry::TVec4< A >&);

        template < typename A >
        void set(A _x, A _y, A _z, A _w) NO_INLINE {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        void scale(T val);
    };

    template < typename T >
    struct TQuat4 : public TVec4< T > {
    public:
        /* Constructors */
        inline TQuat4() {}

        inline TQuat4(T xyz, T _w) {
            this->x = xyz;
            this->y = xyz;
            this->z = xyz;
            this->w = _w;
        }

        template < typename A >
        TQuat4(A _x, A _y, A _z, A _w) {
            this->x = _x;
            this->y = _y;
            this->z = _z;
            this->w = _w;
        }

        inline TVec3< T >* toTvec() { return (TVec3< T >*)this; }

        void set(T, T, T, T);

#ifdef __MWERKS__
        template < typename T >
        inline void set(T _x, T _y, T _z, T _w) {
            TVec4< T >::set(_x, _y, _z, _w);
        }
#else
        template < typename >
        inline void set(T _x, T _y, T _z, T _w) {
            TVec4< T >::set(_x, _y, _z, _w);
        }
#endif

        /* General operations */
        void normalize();
        void normalize(const TQuat4< T >& rSrc);

        void getXDir(TVec3< T >& rDest) const {
            rDest.template set< T >(1.0f - this->y * this->y * 2.0f - this->z * this->z * 2.0f, this->x * this->y * 2.0f + this->w * this->z * 2.0f,
                                    this->x * this->z * 2.0f - this->w * this->y * 2.0f);
        }

        void getYDir(TVec3< T >& rDest) const {
            rDest.template set< T >(this->x * this->y * 2.0f - this->w * this->z * 2.0f, 1.0f - this->x * this->x * 2.0f - this->z * this->z * 2.0f,
                                    this->y * this->z * 2.0f + this->w * this->x * 2.0f);
        }

        void getZDir(TVec3< T >& rDest) const {
            rDest.template set< T >(this->x * this->z * 2.0f + this->w * this->y * 2.0f, this->y * this->z * 2.0f - this->w * this->x * 2.0f,
                                    1.0f - this->x * this->x * 2.0f - this->y * this->y * 2.0f);
        }

        void getEuler(TVec3< T >& rDest) const;
        void setEuler(T _x, T _y, T _z);
        void setEulerZ(T _z);

        void setRotate(const TVec3< T >&, const TVec3< T >&, T);
        void setRotate(const TVec3< T >&, const TVec3< T >&);

        void setRotate(const TVec3< T >& pVec, f32 pAngle) {
            f32 halfAngle = pAngle * 0.5f;
            toTvec()->scale(sin(halfAngle), pVec);
            this->w = cos(halfAngle);
        }

        // used in BegoManBase to get the TVec constructor inlined
        void setRotate(f32 x, f32 y, f32 z, f32 pAngle) {
            TVec3< T > pVec(x, y, z);
            setRotate(pVec, pAngle);
        }

        void rotate(TVec3< T >& rDest) const;

        void slerp(const TQuat4< T >& a1, const TQuat4< T >& a2, T a3) {
            this->x = a1.x;
            this->y = a1.y;
            this->z = a1.z;
            this->w = a1.w;
            this->slerp(a2, a3);
        }

        void scale(f32 scalar, const TVec3< T >& rVec) { TVec3< T >::scale(scalar, rVec); }

        void slerp(const TQuat4< T >&, T);
        void transform(const TVec3< T >&, TVec3< T >& rDest);
        void transform(TVec3< T >& rDest) const;

        /* Operators */
        TQuat4< T >& operator=(const TQuat4< T >& rSrc);
    };

};  // namespace JGeometry

typedef JGeometry::TVec2< s16 > TVec2s;
typedef JGeometry::TVec2< f32 > TVec2f;
typedef JGeometry::TVec3< f32 > TVec3f;
typedef JGeometry::TVec3< s16 > TVec3s;
typedef JGeometry::TVec4< f32 > TVec4f;
typedef JGeometry::TQuat4< f32 > TQuat4f;

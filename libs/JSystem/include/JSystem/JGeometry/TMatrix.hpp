#pragma once

#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include <cmath>
#include <revolution/mtx.h>

inline f32 SIN(f32 angle) {
    return sin(angle);
}

inline f32 COS(f32 angle) {
    return cos(angle);
}

namespace JGeometry {
    template < typename T >
    struct SMatrix33R {
        T get(int row, int column) const {
            return mMtx[column][row];
        }

        void set(T xx, T xy, T xz, T yx, T yy, T yz, T zx, T zy, T zz) {
            mMtx[0][0] = xx;
            mMtx[1][0] = xy;
            mMtx[2][0] = xz;
            mMtx[0][1] = yx;
            mMtx[1][1] = yy;
            mMtx[2][1] = yz;
            mMtx[0][2] = zx;
            mMtx[1][2] = zy;
            mMtx[2][2] = zz;
        }

        T mMtx[3][3];
    };

    template < typename T >
    inline T max(T a, T b) {
        return a >= b ? a : b;
    }

    template < typename T >
    struct SMatrix34C {
    public:
        typedef f32 ArrType[4];
        void set(const ArrType* pSrc) {
            JMath::gekko_ps_copy12(this, pSrc);
        }

        void set(const SMatrix34C< T >& rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        void set(T xx, T xy, T xz, T tx, T yx, T yy, T yz, T ty, T zx, T zy, T zz, T tz) NO_INLINE {
            mMtx[0][0] = xx;
            mMtx[0][1] = xy;
            mMtx[0][2] = xz;
            mMtx[0][3] = tx;
            mMtx[1][0] = yx;
            mMtx[1][1] = yy;
            mMtx[1][2] = yz;
            mMtx[1][3] = ty;
            mMtx[2][0] = zx;
            mMtx[2][1] = zy;
            mMtx[2][2] = zz;
            mMtx[2][3] = tz;
        }

        void scale(T scale) {
            mMtx[0][0] *= scale;
            mMtx[0][1] *= scale;
            mMtx[0][2] *= scale;
            mMtx[1][0] *= scale;
            mMtx[1][1] *= scale;
            mMtx[1][2] *= scale;
            mMtx[2][0] *= scale;
            mMtx[2][1] *= scale;
            mMtx[2][2] *= scale;
        }

        inline void setInline(const SMatrix34C< T >& rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        inline void setInline(const SMatrix34C< T >* rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        inline void setInline(const MtxPtr rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        inline void scaleInline(T scalar) {
            mMtx[0][0] *= scalar;
            mMtx[0][1] *= scalar;
            mMtx[0][2] *= scalar;
            mMtx[1][0] *= scalar;
            mMtx[1][1] *= scalar;
            mMtx[1][2] *= scalar;
            mMtx[2][0] *= scalar;
            mMtx[2][1] *= scalar;
            mMtx[2][2] *= scalar;
        }

        inline Mtx* toMtx() {
            return (Mtx*)mMtx;
        }

        inline const Mtx* toCMtx() const {
            return (const Mtx*)mMtx;
        }

        inline MtxPtr toMtxPtr() {
            return (MtxPtr)mMtx;
        }

        operator ArrType*() {
            return mMtx;
        }

        operator const ArrType*() const {
            return mMtx;
        }

        f32 get(int x, int y) const {
            return mMtx[x][y];
        }

        f32 operator()(int x, int y) const {
            return get(x, y);
        }

        ALWAYS_INLINE inline f32 dot() const {
            return ((this->mMtx[1][0] * this->mMtx[1][0]) + (this->mMtx[0][0] * this->mMtx[0][0]) + (this->mMtx[2][0] * this->mMtx[2][0]) +
                    (this->mMtx[0][1] * this->mMtx[0][1]) + (this->mMtx[1][1] * this->mMtx[1][1]) + (this->mMtx[2][1] * this->mMtx[2][1]) +
                    (this->mMtx[0][2] * this->mMtx[0][2]) + (this->mMtx[1][2] * this->mMtx[1][2]) + (this->mMtx[2][2] * this->mMtx[2][2]));
        }

        inline f32 dot(int x, int y) {
            return (this->mMtx[x][y] * this->mMtx[x][y]);
        }

        inline f32 dotX() const {
            // f32 _10 = ;
            // f32 _20 = ;
            // f32 _00 = ;

            return (this->mMtx[1][0] * this->mMtx[1][0]) + (this->mMtx[0][0] * this->mMtx[0][0]) + (this->mMtx[2][0] * this->mMtx[2][0]);
        }

        inline f32 dotY() const {
            // f32 _11 = ;
            // f32 _21 = ;
            // f32 _01 = ;

            return (this->mMtx[1][1] * this->mMtx[1][1]) + (this->mMtx[0][1] * this->mMtx[0][1]) + (this->mMtx[2][1] * this->mMtx[2][1]);
        }

        inline f32 dotZ() const {
            // f32 _12 = ;
            // f32 _22 = ;
            // f32 _02 = ;

            return (this->mMtx[1][2] * this->mMtx[1][2]) + (this->mMtx[0][2] * this->mMtx[0][2]) + (this->mMtx[2][2] * this->mMtx[2][2]);
        }

        inline f32 dot(int x, int y) const {
            return (this->mMtx[x][y] * this->mMtx[x][y]);
        }

        T mMtx[3][4];
    };

    template < typename T >
    struct TMatrix34 : public T {
    public:
        void identity() NO_INLINE {
            this->mMtx[1][0] = this->mMtx[2][0] = this->mMtx[0][1] = this->mMtx[2][1] = this->mMtx[0][2] = this->mMtx[1][2] = this->mMtx[0][3] =
                this->mMtx[1][3] = this->mMtx[2][3] = 0.0f;
            this->mMtx[0][0] = this->mMtx[1][1] = this->mMtx[2][2] = 1.0f;
        }

        void concat(const T& rSrcA, const T& rSrcB) NO_INLINE {
            this->set(rSrcA(0, 2) * rSrcB(2, 0) + (rSrcA(0, 0) * rSrcB(0, 0) + rSrcA(0, 1) * rSrcB(1, 0)),
                      rSrcA(0, 2) * rSrcB(2, 1) + (rSrcA(0, 0) * rSrcB(0, 1) + rSrcA(0, 1) * rSrcB(1, 1)),
                      rSrcA(0, 2) * rSrcB(2, 2) + (rSrcA(0, 0) * rSrcB(0, 2) + rSrcA(0, 1) * rSrcB(1, 2)),
                      rSrcA(0, 3) + (rSrcA(0, 2) * rSrcB(2, 3) + (rSrcA(0, 0) * rSrcB(0, 3) + rSrcA(0, 1) * rSrcB(1, 3))),
                      rSrcA(1, 2) * rSrcB(2, 0) + (rSrcA(1, 0) * rSrcB(0, 0) + rSrcA(1, 1) * rSrcB(1, 0)),
                      rSrcA(1, 2) * rSrcB(2, 1) + (rSrcA(1, 0) * rSrcB(0, 1) + rSrcA(1, 1) * rSrcB(1, 1)),
                      rSrcA(1, 2) * rSrcB(2, 2) + (rSrcA(1, 0) * rSrcB(0, 2) + rSrcA(1, 1) * rSrcB(1, 2)),
                      rSrcA(1, 3) + (rSrcA(1, 2) * rSrcB(2, 3) + (rSrcA(1, 0) * rSrcB(0, 3) + rSrcA(1, 1) * rSrcB(1, 3))),
                      rSrcA(2, 2) * rSrcB(2, 0) + (rSrcA(2, 0) * rSrcB(0, 0) + rSrcA(2, 1) * rSrcB(1, 0)),
                      rSrcA(2, 2) * rSrcB(2, 1) + (rSrcA(2, 0) * rSrcB(0, 1) + rSrcA(2, 1) * rSrcB(1, 1)),
                      rSrcA(2, 2) * rSrcB(2, 2) + (rSrcA(2, 0) * rSrcB(0, 2) + rSrcA(2, 1) * rSrcB(1, 2)),
                      rSrcA(2, 3) + (rSrcA(2, 2) * rSrcB(2, 3) + (rSrcA(2, 0) * rSrcB(0, 3) + rSrcA(2, 1) * rSrcB(1, 3))));
        }

        void concat(const T& rSrc) NO_INLINE {
            this->set((*this)(0, 2) * rSrc(2, 0) + ((*this)(0, 0) * rSrc(0, 0) + (*this)(0, 1) * rSrc(1, 0)),
                      (*this)(0, 2) * rSrc(2, 1) + ((*this)(0, 0) * rSrc(0, 1) + (*this)(0, 1) * rSrc(1, 1)),
                      (*this)(0, 2) * rSrc(2, 2) + ((*this)(0, 0) * rSrc(0, 2) + (*this)(0, 1) * rSrc(1, 2)),
                      (*this)(0, 3) + ((*this)(0, 2) * rSrc(2, 3) + ((*this)(0, 0) * rSrc(0, 3) + (*this)(0, 1) * rSrc(1, 3))),
                      (*this)(1, 2) * rSrc(2, 0) + ((*this)(1, 0) * rSrc(0, 0) + (*this)(1, 1) * rSrc(1, 0)),
                      (*this)(1, 2) * rSrc(2, 1) + ((*this)(1, 0) * rSrc(0, 1) + (*this)(1, 1) * rSrc(1, 1)),
                      (*this)(1, 2) * rSrc(2, 2) + ((*this)(1, 0) * rSrc(0, 2) + (*this)(1, 1) * rSrc(1, 2)),
                      (*this)(1, 3) + ((*this)(1, 2) * rSrc(2, 3) + ((*this)(1, 0) * rSrc(0, 3) + (*this)(1, 1) * rSrc(1, 3))),
                      (*this)(2, 2) * rSrc(2, 0) + ((*this)(2, 0) * rSrc(0, 0) + (*this)(2, 1) * rSrc(1, 0)),
                      (*this)(2, 2) * rSrc(2, 1) + ((*this)(2, 0) * rSrc(0, 1) + (*this)(2, 1) * rSrc(1, 1)),
                      (*this)(2, 2) * rSrc(2, 2) + ((*this)(2, 0) * rSrc(0, 2) + (*this)(2, 1) * rSrc(1, 2)),
                      (*this)(2, 3) + ((*this)(2, 2) * rSrc(2, 3) + ((*this)(2, 0) * rSrc(0, 3) + (*this)(2, 1) * rSrc(1, 3))));
        }

        f32 determinant33() const {
            return (*this)(0, 0) * ((*this)(1, 1) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 1)) -
                   (*this)(0, 1) * ((*this)(1, 0) * (*this)(2, 2) - (*this)(1, 2) * (*this)(2, 0)) +
                   (*this)(0, 2) * ((*this)(1, 0) * (*this)(2, 1) - (*this)(1, 1) * (*this)(2, 0));
        }

        bool invert(const TMatrix34< T >& rSrc) NO_INLINE {
            f32 determinant = rSrc.determinant33();

            if (determinant <= TUtil< f32 >::epsilon() && determinant >= -TUtil< f32 >::epsilon()) {
                return false;
            }

            SMatrix33R< f32 > inverse;
            f32 scale = TUtil< f32 >::invert(determinant);

            TVec3f translation;
            translation.set(rSrc(0, 3), rSrc(1, 3), rSrc(2, 3));
            translation.x = -translation.x;
            translation.y = -translation.y;
            translation.z = -translation.z;

            inverse.set(scale * (rSrc(1, 1) * rSrc(2, 2) - rSrc(1, 2) * rSrc(2, 1)), scale * -(rSrc(0, 1) * rSrc(2, 2) - rSrc(0, 2) * rSrc(2, 1)),
                        scale * (rSrc(0, 1) * rSrc(1, 2) - rSrc(0, 2) * rSrc(1, 1)), scale * -(rSrc(1, 0) * rSrc(2, 2) - rSrc(1, 2) * rSrc(2, 0)),
                        scale * (rSrc(0, 0) * rSrc(2, 2) - rSrc(0, 2) * rSrc(2, 0)), scale * -(rSrc(0, 0) * rSrc(1, 2) - rSrc(0, 2) * rSrc(1, 0)),
                        scale * (rSrc(1, 0) * rSrc(2, 1) - rSrc(1, 1) * rSrc(2, 0)), scale * -(rSrc(0, 0) * rSrc(2, 1) - rSrc(0, 1) * rSrc(2, 0)),
                        scale * (rSrc(0, 0) * rSrc(1, 1) - rSrc(0, 1) * rSrc(1, 0)));

            translation.set< f32 >(translation.x * inverse.mMtx[0][0] + translation.y * inverse.mMtx[1][0] + translation.z * inverse.mMtx[2][0],
                                   translation.x * inverse.mMtx[0][1] + translation.y * inverse.mMtx[1][1] + translation.z * inverse.mMtx[2][1],
                                   translation.x * inverse.mMtx[0][2] + translation.y * inverse.mMtx[1][2] + translation.z * inverse.mMtx[2][2]);
            this->mMtx[0][0] = inverse.get(0, 0);
            this->mMtx[1][0] = inverse.get(1, 0);
            this->mMtx[2][0] = inverse.get(2, 0);
            this->mMtx[0][1] = inverse.get(0, 1);
            this->mMtx[1][1] = inverse.get(1, 1);
            this->mMtx[2][1] = inverse.get(2, 1);
            this->mMtx[0][2] = inverse.get(0, 2);
            this->mMtx[1][2] = inverse.get(1, 2);
            this->mMtx[2][2] = inverse.get(2, 2);
            this->mMtx[0][3] = translation.x;
            this->mMtx[1][3] = translation.y;
            this->mMtx[2][3] = translation.z;
            return true;
        }

        void mult(const TVec3f& rSrc, TVec3f& rDest) const NO_INLINE {
            rDest.set< f32 >((*this)(0, 3) + (rSrc.z * (*this)(0, 2) + (rSrc.x * (*this)(0, 0) + rSrc.y * (*this)(0, 1))),
                             (*this)(1, 3) + (rSrc.z * (*this)(1, 2) + (rSrc.x * (*this)(1, 0) + rSrc.y * (*this)(1, 1))),
                             (*this)(2, 3) + (rSrc.z * (*this)(2, 2) + (rSrc.x * (*this)(2, 0) + rSrc.y * (*this)(2, 1))));
        }

        void multTranspose(const TVec3f& rSrc, TVec3f& rDest) const NO_INLINE {
            TVec3f translated;
            translated.x = rSrc.x - (*this)(0, 3);
            translated.y = rSrc.y - (*this)(1, 3);
            translated.z = rSrc.z - (*this)(2, 3);

            rDest.set< f32 >(translated.z * (*this)(2, 0) + (translated.x * (*this)(0, 0) + translated.y * (*this)(1, 0)),
                             translated.z * (*this)(2, 1) + (translated.x * (*this)(0, 1) + translated.y * (*this)(1, 1)),
                             translated.z * (*this)(2, 2) + (translated.x * (*this)(0, 2) + translated.y * (*this)(1, 2)));
        }

        void scale(f32 scalar) {
            this->mMtx[0][0] *= scalar;
            this->mMtx[1][0] *= scalar;
            this->mMtx[2][0] *= scalar;
            this->mMtx[0][1] *= scalar;
            this->mMtx[1][1] *= scalar;
            this->mMtx[2][1] *= scalar;
            this->mMtx[0][2] *= scalar;
            this->mMtx[1][2] *= scalar;
            this->mMtx[2][2] *= scalar;
            this->mMtx[0][3] *= scalar;
            this->mMtx[1][3] *= scalar;
            this->mMtx[2][3] *= scalar;
        }

        void scaleXYZ(f32 scalar) {
            T::scale(scalar);
        }
    };
    template < class T >
    struct TRotation3 : public T {
    public:
        void identity33() {
            this->mMtx[0][0] = 1.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[2][0] = 0.0f;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[2][2] = 1.0f;
        };

        void getXDir(TVec3f& rDest) const {
            f32 z = this->mMtx[2][0];
            f32 y = this->mMtx[1][0];
            f32 x = this->mMtx[0][0];
            rDest.set< f32 >(x, y, z);
        };

        void getYDir(TVec3f& rDest) const {
            f32 z = this->mMtx[2][1];
            f32 y = this->mMtx[1][1];
            f32 x = this->mMtx[0][1];
            rDest.set< f32 >(x, y, z);
        };

        void getZDir(TVec3f& rDest) const {
            f32 z = this->mMtx[2][2];
            f32 y = this->mMtx[1][2];
            f32 x = this->mMtx[0][2];
            rDest.set< f32 >(x, y, z);
        };

        void getXYZDir(TVec3f& rDestX, TVec3f& rDestY, TVec3f& rDestZ) const {
            f32 xz = this->mMtx[2][0];
            f32 xy = this->mMtx[1][0];
            f32 xx = this->mMtx[0][0];
            rDestX.set< f32 >(xx, xy, xz);
            f32 yz = this->mMtx[2][1];
            f32 yy = this->mMtx[1][1];
            f32 yx = this->mMtx[0][1];
            rDestY.set< f32 >(yx, yy, yz);
            f32 zz = this->mMtx[2][2];
            f32 zy = this->mMtx[1][2];
            f32 zx = this->mMtx[0][2];
            rDestZ.set< f32 >(zx, zy, zz);
        }

        void setXDir(const TVec3f& rSrc) {
            this->mMtx[0][0] = rSrc.x;
            this->mMtx[1][0] = rSrc.y;
            this->mMtx[2][0] = rSrc.z;
        }

        void setXDir(f32 x, f32 y, f32 z) NO_INLINE {
            this->mMtx[0][0] = x;
            this->mMtx[1][0] = y;
            this->mMtx[2][0] = z;
        }

        void setYDir(const TVec3f& rSrc) {
            this->mMtx[0][1] = rSrc.x;
            this->mMtx[1][1] = rSrc.y;
            this->mMtx[2][1] = rSrc.z;
        }

        void setYDir(f32 x, f32 y, f32 z) NO_INLINE {
            this->mMtx[0][1] = x;
            this->mMtx[1][1] = y;
            this->mMtx[2][1] = z;
        }

        void setZDir(const TVec3f& rSrc) {
            this->mMtx[0][2] = rSrc.x;
            this->mMtx[1][2] = rSrc.y;
            this->mMtx[2][2] = rSrc.z;
        }

        void setZDir(f32 x, f32 y, f32 z) NO_INLINE {
            this->mMtx[0][2] = x;
            this->mMtx[1][2] = y;
            this->mMtx[2][2] = z;
        }

        void setXYZDir(const TVec3f& rSrcX, const TVec3f& rSrcY, const TVec3f& rSrcZ) {
            this->mMtx[0][0] = rSrcX.x;
            this->mMtx[1][0] = rSrcX.y;
            this->mMtx[2][0] = rSrcX.z;
            this->mMtx[0][1] = rSrcY.x;
            this->mMtx[1][1] = rSrcY.y;
            this->mMtx[2][1] = rSrcY.z;
            this->mMtx[0][2] = rSrcZ.x;
            this->mMtx[1][2] = rSrcZ.y;
            this->mMtx[2][2] = rSrcZ.z;
        }

        inline void getYDir2(TVec3f& rDest) const {
            f32 x = this->mMtx[0][1];
            f32 y = this->mMtx[1][1];
            f32 z = this->mMtx[2][1];
            rDest.set< f32 >(x, y, z);
        }

        inline void getZDir2(TVec3f& rDest) const {
            rDest.set< f32 >(this->mMtx[0][2], this->mMtx[1][2], this->mMtx[2][2]);
        }

        inline void setXYZDir2(const TVec3f& rSrcX, const TVec3f& rSrcY, const TVec3f& rSrcZ) {
            this->mMtx[0][0] = rSrcX.x;
            this->mMtx[0][1] = rSrcX.y;
            this->mMtx[0][2] = rSrcX.z;
            this->mMtx[1][0] = rSrcY.x;
            this->mMtx[1][1] = rSrcY.y;
            this->mMtx[1][2] = rSrcY.z;
            this->mMtx[2][0] = rSrcZ.x;
            this->mMtx[2][1] = rSrcZ.y;
            this->mMtx[2][2] = rSrcZ.z;
        }

        void getEulerDegree(TVec3f& rDest) const {
            TVec3f rot;
            getEulerXYZ(rot);
            rDest.set< f32 >(rot.x * (180.0f / PI), rot.y * (180.0f / PI), rot.z * (180.0f / PI));
        }

        void getEuler(TVec3f& rDest) const {
            getEulerXYZ(rDest);
        }

        void getEulerXYZ(TVec3f& rDest) const {
            const f32 zx = this->mMtx[2][0];
            if (zx - 1.0f >= -(f32)JGeometry::TUtil< f32 >::epsilon()) {
                rDest.set(JMAATan2(-this->mMtx[0][1], this->mMtx[1][1]), -HALF_PI, 0.0f);
                return;
            }

            f32 f1 = 1.0f;  // TODO: this is a hack to fix the float regswap
            if (zx + f1 <= (f32)JGeometry::TUtil< f32 >::epsilon()) {
                rDest.set(JMAATan2(this->mMtx[0][1], this->mMtx[1][1]), HALF_PI, 0.0f);
                return;
            }

            rDest.x = JMAATan2(this->mMtx[2][1], this->mMtx[2][2]);
            rDest.z = JMAATan2(this->mMtx[1][0], this->mMtx[0][0]);
            rDest.y = JGeometry::TUtil< f32 >::asin(-this->mMtx[2][0]);
        }

        void setEulerX(f32 angle) {
            f32 s = SIN(angle);
            f32 c = COS(angle);

            this->mMtx[0][0] = 1.0f;
            this->mMtx[2][1] = s;
            this->mMtx[1][1] = c;
            this->mMtx[1][2] = -s;
            this->mMtx[2][2] = c;
            this->mMtx[2][0] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[0][1] = 0.0f;
        }

        void setEulerY(f32 angle) {
            f32 s = SIN(angle);
            f32 c = COS(angle);

            this->mMtx[0][2] = s;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[0][0] = c;
            this->mMtx[2][0] = -s;
            this->mMtx[2][2] = c;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[0][1] = 0.0f;
        }

        void setEulerZ(f32 angle) {
            f32 s = SIN(angle);
            f32 c = COS(angle);

            this->mMtx[1][0] = s;
            this->mMtx[0][0] = c;
            this->mMtx[0][1] = -s;
            this->mMtx[1][1] = c;
            this->mMtx[2][2] = 1.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[2][0] = 0.0f;
            this->mMtx[0][2] = 0.0f;
        }

        void getQuat(TQuat4f& rDest) const NO_INLINE {
            if ((*this)(0, 0) + (*this)(1, 1) + (*this)(2, 2) >= 0.0f) {
                f32 root = (*this)(0, 0) + (*this)(1, 1) + (*this)(2, 2);
                root += 1.0f;
                root = TUtil< f32 >::sqrt(root);
                f32 scale = 0.5f / root;
                rDest.w = 0.5f * root;
                rDest.x = scale * ((*this)(2, 1) - (*this)(1, 2));
                rDest.y = scale * ((*this)(0, 2) - (*this)(2, 0));
                rDest.z = scale * ((*this)(1, 0) - (*this)(0, 1));
            } else {
                f32 maximum = JGeometry::max(((*this)(0, 0) >= (*this)(1, 1) ? (*this)(0, 0) : (*this)(1, 1)), (*this)(2, 2));

                if (maximum == (*this)(0, 0)) {
                    f32 root = TUtil< f32 >::sqrt(1.0f + ((*this)(0, 0) - ((*this)(1, 1) + (*this)(2, 2))));
                    f32 scale = 0.5f / root;
                    rDest.x = 0.5f * root;
                    rDest.y = scale * ((*this)(0, 1) + (*this)(1, 0));
                    rDest.z = scale * ((*this)(2, 0) + (*this)(0, 2));
                    rDest.w = scale * ((*this)(2, 1) - (*this)(1, 2));
                } else if (maximum == (*this)(1, 1)) {
                    f32 root = TUtil< f32 >::sqrt(1.0f + ((*this)(1, 1) - ((*this)(2, 2) + (*this)(0, 0))));
                    f32 scale = 0.5f / root;
                    rDest.y = 0.5f * root;
                    rDest.z = scale * ((*this)(1, 2) + (*this)(2, 1));
                    rDest.x = scale * ((*this)(0, 1) + (*this)(1, 0));
                    rDest.w = scale * ((*this)(0, 2) - (*this)(2, 0));
                } else {
                    f32 root = TUtil< f32 >::sqrt(1.0f + ((*this)(2, 2) - ((*this)(0, 0) + (*this)(1, 1))));
                    f32 scale = 0.5f / root;
                    rDest.z = 0.5f * root;
                    rDest.x = scale * ((*this)(2, 0) + (*this)(0, 2));
                    rDest.y = scale * ((*this)(1, 2) + (*this)(2, 1));
                    rDest.w = scale * ((*this)(1, 0) - (*this)(0, 1));
                }
            }
        }

        void setQuat(const TQuat4f& q) {
            f32 yy = 2.0f * q.y * q.y;
            f32 zz = 2.0f * q.z * q.z;
            f32 xx = 2.0f * q.x * q.x;

            f32 xy = 2.0f * q.x * q.y;
            f32 xz = 2.0f * q.x * q.z;
            f32 yz = 2.0f * q.y * q.z;

            f32 wz = 2.0f * q.w * q.z;
            f32 wx = 2.0f * q.w * q.x;
            f32 wy = 2.0f * q.w * q.y;

            this->mMtx[0][0] = 1.0f - yy - zz;
            this->mMtx[0][1] = xy - wz;
            this->mMtx[0][2] = xz + wy;

            this->mMtx[1][0] = xy + wz;
            this->mMtx[1][1] = 1.0f - xx - zz;
            this->mMtx[1][2] = yz - wx;

            this->mMtx[2][0] = xz - wy;
            this->mMtx[2][1] = yz + wx;
            this->mMtx[2][2] = 1.0f - xx - yy;
        }

        f32 getRotate(TVec3f& rAxis) const {
            TQuat4f rot;
            getQuat(rot);
            return (f64)rot.getRotate(rAxis);
        }

        void getScale(TVec3f& rDest) const {
            {
                f32 y = this->mMtx[1][0];
                f32 x = this->mMtx[0][0];
                f32 z = this->mMtx[2][0];
                rDest.x = TUtil< f32 >::sqrt(x * x + y * y + z * z);
            }

            {
                f32 y = this->mMtx[1][1];
                f32 x = this->mMtx[0][1];
                f32 z = this->mMtx[2][1];
                rDest.y = TUtil< f32 >::sqrt(x * x + y * y + z * z);
            }

            {
                f32 y = this->mMtx[1][2];
                f32 x = this->mMtx[0][2];
                f32 z = this->mMtx[2][2];
                rDest.z = TUtil< f32 >::sqrt(x * x + y * y + z * z);
            }
        }

        void setScale(const TVec3f& rSrc);
        void setScale(f32 x, f32 y, f32 z) NO_INLINE {
            this->mMtx[0][0] = x;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[1][1] = y;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[2][0] = 0.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[2][2] = z;
        }

        void setRotate(const TVec3f& rAxis, f32 angle) {
            TVec3f vec;
            vec.normalize(rAxis);

            f32 s = SIN(angle);
            f32 c = COS(angle);

            f32 negc = 1.0f - c;

            f32 x, y, z;

            x = vec.x;
            y = vec.y;
            z = vec.z;

            f32 xx, yy, zz;
            xx = x * x;
            yy = y * y;
            zz = z * z;

            this->mMtx[0][0] = c + negc * xx;
            this->mMtx[0][1] = negc * x * y - s * z;
            this->mMtx[0][2] = negc * x * z + s * y;
            this->mMtx[1][0] = negc * x * y + s * z;
            this->mMtx[1][1] = c + negc * yy;
            this->mMtx[1][2] = negc * y * z - s * x;
            this->mMtx[2][0] = negc * x * z - s * y;
            this->mMtx[2][1] = negc * y * z + s * x;
            this->mMtx[2][2] = c + negc * zz;
        }

        void setRotateDegree(const TVec3f& rAxis, f32 angle) {
            TVec3f vec;
            vec.normalize(rAxis);

            f32 s = SIN(angle * (PI / 180.0f));
            f32 c = COS(angle * (PI / 180.0f));

            f32 negc = 1.0f - c;

            f32 x, y, z;

            x = vec.x;
            y = vec.y;
            z = vec.z;

            f32 xx, yy, zz;
            xx = x * x;
            yy = y * y;
            zz = z * z;

            this->mMtx[0][0] = c + negc * xx;
            this->mMtx[0][1] = negc * x * y - s * z;
            this->mMtx[0][2] = negc * x * z + s * y;
            this->mMtx[1][0] = negc * x * y + s * z;
            this->mMtx[1][1] = c + negc * yy;
            this->mMtx[1][2] = negc * y * z - s * x;
            this->mMtx[2][0] = negc * x * z - s * y;
            this->mMtx[2][1] = negc * y * z + s * x;
            this->mMtx[2][2] = c + negc * zz;
        }

        void setRotate(const TVec3f& rFrom, const TVec3f& rTo) {
            TQuat4f q;
            q.setRotate(rFrom, rTo);
            setQuat(q);
        };

        void setRotateDegree(const TVec3f& rRot) {
            setRotate(rRot * (PI / 180.0f));
        }

        void setRotate(const TVec3f& rRot) {
            f32 z = rRot.z;
            f32 y = rRot.y;
            f32 x = rRot.x;
            setRotate(x, y, z);
        }

        void setEuler(const TVec3f& rRot) {
            setRotate(rRot);
        }

        void setRotate(f32 rx, f32 ry, f32 rz) {
            // NOTE: setEulerXYZ?
            f32 sinX, sinY, sinZ;
            f32 cosX, cosY, cosZ;

            cosZ = COS(rz);
            cosY = COS(ry);
            cosX = COS(rx);
            sinZ = SIN(rz);
            sinY = SIN(ry);
            sinX = SIN(rx);

            f32 sXsY = sinX * sinY;
            f32 cXcZ = cosX * cosZ;
            f32 cXsZ = cosX * sinZ;

            this->mMtx[0][0] = cosY * cosZ;
            this->mMtx[1][0] = cosY * sinZ;
            this->mMtx[2][0] = -sinY;

            this->mMtx[0][1] = sXsY * cosZ - cosX * sinZ;
            this->mMtx[1][1] = cosX * cosZ + sXsY * sinZ;
            this->mMtx[2][1] = sinX * cosY;

            this->mMtx[0][2] = cXcZ * sinY + sinX * sinZ;
            this->mMtx[1][2] = cXsZ * sinY - sinX * cosZ;
            this->mMtx[2][2] = cosX * cosY;
        }

        void mult33(TVec3f& rVec) const {
            mult33(rVec, rVec);
        }

        void mult33(const TVec3f& rSrc, TVec3f& rDst) const {
            rDst.set< f32 >(rSrc.x * get(0, 0) + rSrc.y * get(0, 1) + rSrc.z * get(0, 2),

                            rSrc.x * get(1, 0) + rSrc.y * get(1, 1) + rSrc.z * get(1, 2),

                            rSrc.x * get(2, 0) + rSrc.y * get(2, 1) + rSrc.z * get(2, 2));
        }
    };

    template < class T >
    struct TPosition3 : public TRotation3< T > {
    public:
        TPosition3() {};

        TPosition3(MtxPtr rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        TPosition3(const TPosition3< T >* rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        void getTrans(TVec3f& rDest) const {
            f32 z = this->mMtx[2][3];
            f32 y = this->mMtx[1][3];
            f32 x = this->mMtx[0][3];
            rDest.set< f32 >(x, y, z);
        }

        inline void getTransInline2(TVec3f& rDest) const {
            f32 x = this->mMtx[0][3];
            f32 y = this->mMtx[1][3];
            f32 z = this->mMtx[2][3];
            rDest.set< f32 >(x, y, z);
        }

        void setTrans(const TVec3f& rSrc) {
            this->mMtx[0][3] = rSrc.x;
            this->mMtx[1][3] = rSrc.y;
            this->mMtx[2][3] = rSrc.z;
        }

        void setTrans(f32 x, f32 y, f32 z) {
            this->mMtx[0][3] = x;
            this->mMtx[1][3] = y;
            this->mMtx[2][3] = z;
        }

        void zeroTrans() {
            this->mMtx[0][3] = 0.0f;
            this->mMtx[1][3] = 0.0f;
            this->mMtx[2][3] = 0.0f;
        }

        inline void zeroTransInline2() {
            this->mMtx[0][3] = 0.0f;
            this->mMtx[1][3] = 0.0f;
            this->mMtx[2][3] = 0.0f;
        }

        inline void makeTrans(const TVec3f& rTrans) {
            this->identity();
            setTrans(rTrans);
        }

        inline void makeTrans(f32 x, f32 y, f32 z) {
            this->mMtx[0][0] = 1.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[2][0] = 0.0f;

            this->mMtx[0][1] = 0.0f;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[2][1] = 0.0f;

            this->mMtx[0][2] = 0.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[2][2] = 1.0f;

            this->mMtx[0][3] = x;
            this->mMtx[1][3] = y;
            this->mMtx[2][3] = z;
        }

        void setTR(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, const TVec3f& a4) {
            this->mMtx[0][0] = a1.x;
            this->mMtx[1][0] = a1.y;
            this->mMtx[2][0] = a1.z;

            this->mMtx[0][1] = a2.x;
            this->mMtx[1][1] = a2.y;
            this->mMtx[2][1] = a2.z;

            this->mMtx[0][2] = a3.x;
            this->mMtx[1][2] = a3.y;
            this->mMtx[2][2] = a3.z;

            this->mMtx[0][3] = a4.x;
            this->mMtx[1][3] = a4.y;
            this->mMtx[2][3] = a4.z;
        }

        void makeRotate(const TVec3f& rVec, f32 angle) {
            zeroTrans();
            TRotation3< T >::setRotate(rVec, angle);
        }

        void makeRotateDegree(const TVec3f& rVec, f32 angle) {
            zeroTrans();
            TRotation3< T >::setRotateDegree(rVec, angle);
        }

        void makeRotate(const TVec3f& rFrom, const TVec3f& rTo, f32 angle) {
            TQuat4f q;
            q.setRotate(rFrom, rTo, rFrom.turnRate(rTo, angle));
            makeQuat(q);
        }

        void makeRotateRate(const TVec3f& rFrom, const TVec3f& rTo, f32 ratio) {
            TQuat4f q;
            q.setRotate(rFrom, rTo, ratio);
            makeQuat(q);
        }

        void makeQuat(const TQuat4f& rSrcQuat) {
            zeroTrans();
            TRotation3< T >::setQuat(rSrcQuat);
        }

        void setPositionFromLookAt(const TPosition3< T >& rLookAt) {
            this->mMtx[0][0] = -rLookAt.get(0, 0);
            this->mMtx[1][1] = rLookAt.get(1, 1);
            this->mMtx[2][2] = -rLookAt.get(2, 2);

            f32 yx = rLookAt.get(1, 0);
            this->mMtx[1][0] = -rLookAt.get(0, 1);
            this->mMtx[0][1] = yx;

            f32 zx = rLookAt.get(2, 0);
            this->mMtx[2][0] = -rLookAt.get(0, 2);
            this->mMtx[0][2] = -zx;

            f32 yz = rLookAt.get(1, 2);
            this->mMtx[1][2] = -rLookAt.get(2, 1);
            this->mMtx[2][1] = yz;

            TVec3f pos;
            rLookAt.getTrans(pos);

            this->mMtx[0][3] = pos.x * get(0, 0) - pos.y * get(0, 1) + pos.z * get(0, 2);
            this->mMtx[1][3] = pos.x * get(1, 0) - pos.y * get(1, 1) + pos.z * get(1, 2);
            this->mMtx[2][3] = pos.x * get(2, 0) - pos.y * get(2, 1) + pos.z * get(2, 2);
        }

        void setPositionFromLookAt(const TVec3f& rLookAtPos, const TVec3f& rUp, const TVec3f& rPos) {
            TVec3f aim;
            aim.sub(rPos, rLookAtPos);

            TVec3f front, up, side;
            front.set(aim);
            front.normalize();
            front.negate();

            side.cross(rUp, front);
            up.cross(front, side);
            side.normalize();
            up.normalize();

            this->mMtx[0][0] = side.x;
            this->mMtx[0][1] = side.y;
            this->mMtx[0][2] = side.z;
            this->mMtx[1][0] = up.x;
            this->mMtx[1][1] = up.y;
            this->mMtx[1][2] = up.z;
            this->mMtx[2][0] = front.x;
            this->mMtx[2][1] = front.y;
            this->mMtx[2][2] = front.z;

            this->mMtx[0][3] = rLookAtPos.x * -this->mMtx[0][0] - rLookAtPos.y * this->mMtx[0][1] - rLookAtPos.z * this->mMtx[0][2];
            this->mMtx[1][3] = rLookAtPos.x * -this->mMtx[1][0] - rLookAtPos.y * this->mMtx[1][1] - rLookAtPos.z * this->mMtx[1][2];
            this->mMtx[2][3] = rLookAtPos.x * -this->mMtx[2][0] - rLookAtPos.y * this->mMtx[2][1] - rLookAtPos.z * this->mMtx[2][2];
        }

        void setQT(const TQuat4f& rSrcQuat, const TVec3f& rSrcTrans) {
            TRotation3< T >::setQuat(rSrcQuat);
            setTrans(rSrcTrans);
        }

        void setRTDegree(const TVec3f& rRot, const TVec3f& rSrcTrans) {
            TRotation3< T >::setRotate(rRot);
            setTrans(rSrcTrans);
        }

        void setRT(f32 rx, f32 ry, f32 rz, const TVec3f& rSrcTrans) {
            // nonmatching, see
            // see Mogucchi::updateReferenceMtx

            f32 sinX, sinY, sinZ;
            f32 cosX, cosY, cosZ;

            cosZ = COS(rz);
            cosY = COS(ry);
            cosX = COS(rx);
            sinZ = SIN(rz);
            sinY = SIN(ry);
            sinX = SIN(rx);

            this->mMtx[0][3] = rSrcTrans.x;
            this->mMtx[1][3] = rSrcTrans.y;
            this->mMtx[2][3] = rSrcTrans.z;

            f32 sXsY = sinX * sinY;
            f32 cXcZ = cosX * cosZ;
            f32 cXsZ = cosX * sinZ;

            this->mMtx[0][0] = cosY * cosZ;
            this->mMtx[1][0] = cosY * sinZ;
            this->mMtx[2][0] = -sinY;

            this->mMtx[0][1] = sXsY * cosZ - cosX * sinZ;
            this->mMtx[1][1] = cosX * cosZ + sXsY * sinZ;
            this->mMtx[2][1] = sinX * cosY;

            this->mMtx[0][2] = cXcZ * sinY + sinX * sinZ;
            this->mMtx[1][2] = cXsZ * sinY - sinX * cosZ;
            this->mMtx[2][2] = cosX * cosY;
        }

        f32 get(int x, int y) const {
            return this->mMtx[x][y];
        }

        f32 operator()(int x, int y) const {
            return get(x, y);
        }

        operator TMatrix34< T >() const {
            return *(TMatrix34< T >*)this;
        }

        inline void normalizeBasis() {
            f32 scale = JGeometry::TUtil< f32 >::sqrt(
                (this->get(0, 0) * this->get(0, 0)) + (this->get(1, 0) * this->get(1, 0)) + (this->get(2, 0) * this->get(2, 0)) +
                (this->get(0, 1) * this->get(0, 1)) + (this->get(1, 1) * this->get(1, 1)) + (this->get(2, 1) * this->get(2, 1)) +
                (this->get(0, 2) * this->get(0, 2)) + (this->get(1, 2) * this->get(1, 2)) + (this->get(2, 2) * this->get(2, 2)));

            f32 invLenX = JGeometry::TUtil< f32 >::inv_sqrt((this->get(0, 0) * this->get(0, 0)) + (this->get(1, 0) * this->get(1, 0)) +
                                                            (this->get(2, 0) * this->get(2, 0)));

            this->mMtx[0][0] = invLenX * this->get(0, 0);
            this->mMtx[1][0] = invLenX * this->get(1, 0);
            this->mMtx[2][0] = invLenX * this->get(2, 0);

            f32 invLenY = JGeometry::TUtil< f32 >::inv_sqrt((this->get(0, 1) * this->get(0, 1)) + (this->get(1, 1) * this->get(1, 1)) +
                                                            (this->get(2, 1) * this->get(2, 1)));

            this->mMtx[0][1] = invLenY * this->get(0, 1);
            this->mMtx[1][1] = invLenY * this->get(1, 1);
            this->mMtx[2][1] = invLenY * this->get(2, 1);

            f32 invLenZ = JGeometry::TUtil< f32 >::inv_sqrt((this->get(0, 2) * this->get(0, 2)) + (this->get(1, 2) * this->get(1, 2)) +
                                                            (this->get(2, 2) * this->get(2, 2)));

            this->mMtx[0][2] = invLenZ * this->get(0, 2);
            this->mMtx[1][2] = invLenZ * this->get(1, 2);
            this->mMtx[2][2] = invLenZ * this->get(2, 2);
        }
    };

    template < typename T >
    struct SMatrix44C {
    public:
        typedef f32 ArrType[4];
        void set(const ArrType*);

        void set(const SMatrix44C< T >& rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz, T wx, T wy, T wz, T ww) {
            mMtx[0][0] = rxx;
            mMtx[0][1] = ryx;
            mMtx[0][2] = rzx;
            mMtx[0][3] = tx;

            mMtx[1][0] = rxy;
            mMtx[1][1] = ryy;
            mMtx[1][2] = rzy;
            mMtx[1][3] = ty;

            mMtx[2][0] = rxz;
            mMtx[2][1] = ryz;
            mMtx[2][2] = rzz;
            mMtx[2][3] = tz;

            mMtx[3][0] = wx;
            mMtx[3][1] = wy;
            mMtx[3][2] = wz;
            mMtx[3][3] = ww;
        }

        inline Mtx44Ptr toMtx44() {
            return (Mtx44Ptr)mMtx;
        }

        inline const Mtx44* toCMtx44() const {
            return (const Mtx44*)mMtx;
        }

        inline Mtx44Ptr toMtx44Ptr() {
            return (Mtx44Ptr)mMtx;
        }

        operator ArrType*() {
            return mMtx;
        }

        operator const ArrType*() const {
            return mMtx;
        }

        f32 get(int x, int y) const {
            return mMtx[x][y];
        }

        f32 operator()(int x, int y) const {
            return get(x, y);
        }

        inline void setInline(const Mtx44Ptr rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        inline void setInline(const SMatrix44C< T >& rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        inline void setInline(const SMatrix44C< T >* rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        T mMtx[4][4];
    };

    template < typename T >
    struct TMatrix44 : public T {
    public:
        void identity() {
            this->mMtx[0][0] = 1.0f;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[0][3] = 0.0f;

            this->mMtx[1][0] = 0.0f;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[1][3] = 0.0f;

            this->mMtx[2][0] = 0.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[2][2] = 1.0f;
            this->mMtx[2][3] = 0.0f;

            this->mMtx[3][0] = 0.0f;
            this->mMtx[3][1] = 0.0f;
            this->mMtx[3][2] = 0.0f;
            this->mMtx[3][3] = 1.0f;
        }

        void concat(const T& rA, const T& rB) {
            this->set(rA[0][3] * rB[3][0] + (rA[0][2] * rB[2][0] + (rA[0][0] * rB[0][0] + rA[0][1] * rB[1][0])),
                      rA[0][3] * rB[3][1] + (rA[0][2] * rB[2][1] + (rA[0][0] * rB[0][1] + rA[0][1] * rB[1][1])),
                      rA[0][3] * rB[3][2] + (rA[0][2] * rB[2][2] + (rA[0][0] * rB[0][2] + rA[0][1] * rB[1][2])),
                      rA[0][3] * rB[3][3] + (rA[0][2] * rB[2][3] + (rA[0][0] * rB[0][3] + rA[0][1] * rB[1][3])),
                      rA[1][3] * rB[3][0] + (rA[1][2] * rB[2][0] + (rA[1][0] * rB[0][0] + rA[1][1] * rB[1][0])),
                      rA[1][3] * rB[3][1] + (rA[1][2] * rB[2][1] + (rA[1][0] * rB[0][1] + rA[1][1] * rB[1][1])),
                      rA[1][3] * rB[3][2] + (rA[1][2] * rB[2][2] + (rA[1][0] * rB[0][2] + rA[1][1] * rB[1][2])),
                      rA[1][3] * rB[3][3] + (rA[1][2] * rB[2][3] + (rA[1][0] * rB[0][3] + rA[1][1] * rB[1][3])),
                      rA[2][3] * rB[3][0] + (rA[2][2] * rB[2][0] + (rA[2][0] * rB[0][0] + rA[2][1] * rB[1][0])),
                      rA[2][3] * rB[3][1] + (rA[2][2] * rB[2][1] + (rA[2][0] * rB[0][1] + rA[2][1] * rB[1][1])),
                      rA[2][3] * rB[3][2] + (rA[2][2] * rB[2][2] + (rA[2][0] * rB[0][2] + rA[2][1] * rB[1][2])),
                      rA[2][3] * rB[3][3] + (rA[2][2] * rB[2][3] + (rA[2][0] * rB[0][3] + rA[2][1] * rB[1][3])),
                      rA[3][3] * rB[3][0] + (rA[3][2] * rB[2][0] + (rA[3][0] * rB[0][0] + rA[3][1] * rB[1][0])),
                      rA[3][3] * rB[3][1] + (rA[3][2] * rB[2][1] + (rA[3][0] * rB[0][1] + rA[3][1] * rB[1][1])),
                      rA[3][3] * rB[3][2] + (rA[3][2] * rB[2][2] + (rA[3][0] * rB[0][2] + rA[3][1] * rB[1][2])),
                      rA[3][3] * rB[3][3] + (rA[3][2] * rB[2][3] + (rA[3][0] * rB[0][3] + rA[3][1] * rB[1][3])));
        }

        void concat(const T& rSrc);

        void invert(const TMatrix44< T >& rSrc);

        inline void mult(const TVec3f& rSrc, TVec3f& rDest) const {
            TVec4f pos(rSrc.x * this->mMtx[0][0] + rSrc.z * this->mMtx[0][2],

                       rSrc.y * this->mMtx[1][1] + rSrc.z * this->mMtx[1][2],

                       rSrc.z * this->mMtx[2][2] + this->mMtx[2][3],

                       -rSrc.z);

            rDest.scale(1.0f / pos.w, *pos.toTVec3());
        }
    };

    template < class T >
    struct TProjection3 : public T {
    public:
        TProjection3() {};

        TProjection3(const Mtx44Ptr rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        TProjection3(const TProjection3& rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        f32 getFocalLength(f32 fov) {
            return 1.0f / (f32)::tan(((2.0f * JGeometry::TUtil< f32 >::PI()) * fov) / (360.0f * 2.0f));
        }

        f32 getPower(f32 fov) {
            return ::tan(((2.0f * JGeometry::TUtil< f32 >::PI()) * fov) / (360.0f * 2.0f));
        }

        void makePerspective(f32 fov, f32 aspect, f32 near, f32 far) {
            // FIXME: float regswap
            // https://decomp.me/scratch/hUiFs
            f32 power = ::tan(((2.0f * JGeometry::TUtil< f32 >::PI()) * fov) / (360.0f * 2.0f));
            f32 focalLen = 1.0f / power;
            f32 aperature = focalLen / aspect;
            f32 scale = 1.0f / (far - near);

            this->mMtx[0][0] = aperature;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[0][3] = 0.0f;

            this->mMtx[1][0] = 0.0f;
            this->mMtx[1][1] = focalLen;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[1][3] = 0.0f;

            this->mMtx[2][0] = 0.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[2][2] = -near * scale;
            this->mMtx[2][3] = -(far * near) * scale;

            this->mMtx[3][0] = 0.0f;
            this->mMtx[3][1] = 0.0f;
            this->mMtx[3][2] = -1.0f;
            this->mMtx[3][3] = 0.0f;
        }

        void makeTrans(const TVec2f& offset) {
            this->mMtx[0][0] = 1.0f;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[0][3] = offset.x;

            this->mMtx[1][0] = 0.0f;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[1][3] = offset.y;

            this->mMtx[2][0] = 0.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[2][2] = 1.0f;
            this->mMtx[2][3] = 0.0f;

            this->mMtx[3][0] = 0.0f;
            this->mMtx[3][1] = 0.0f;
            this->mMtx[3][2] = 0.0f;
            this->mMtx[3][3] = 1.0f;
        }

        void makeTrans(f32 offsetX, f32 offsetY) {
            this->mMtx[0][0] = 1.0f;
            this->mMtx[1][0] = 0.0f;
            this->mMtx[2][0] = 0.0f;
            this->mMtx[0][1] = 0.0f;
            this->mMtx[1][1] = 1.0f;
            this->mMtx[2][1] = 0.0f;
            this->mMtx[0][2] = 0.0f;
            this->mMtx[1][2] = 0.0f;
            this->mMtx[2][2] = 1.0f;
            this->mMtx[0][3] = offsetX;
            this->mMtx[1][3] = offsetY;
            this->mMtx[2][3] = 0.0f;
            this->mMtx[3][0] = 0.0f;
            this->mMtx[3][1] = 0.0f;
            this->mMtx[3][2] = 0.0f;
            this->mMtx[3][3] = 1.0f;
        }
    };
};  // namespace JGeometry

typedef JGeometry::SMatrix34C< f32 > TSMtxf;
typedef JGeometry::TMatrix34< TSMtxf > TMtx34f;
typedef JGeometry::TRotation3< TMtx34f > TRot3f;
typedef JGeometry::TPosition3< TMtx34f > TPos3f;
typedef JGeometry::SMatrix44C< f32 > TSMtx44f;
typedef JGeometry::TMatrix44< TSMtx44f > TMtx44f;
typedef JGeometry::TProjection3< TMtx44f > TProj3f;

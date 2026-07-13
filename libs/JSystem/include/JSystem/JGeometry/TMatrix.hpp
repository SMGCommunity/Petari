#pragma once

#include "JSystem/JGeometry/TQuat.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "revolution/mtx.h"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <cmath>

namespace JGeometry {
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
        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz);

        void scale(T scale);

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

        void setTrans(const TVec3f& v) NO_INLINE {
            this->mMtx[0][3] = v.x;
            this->mMtx[1][3] = v.y;
            this->mMtx[2][3] = v.z;
        }

        T mMtx[3][4];
    };

    template < typename T >
    struct TMatrix34 : public T {
    public:
        void identity();
        void concat(const T& rSrcA, const T& rSrcB);
        void concat(const T& rSrc);
        void invert(const TMatrix34< T >& rDest);

        void mult(const TVec3f& rSrc, TVec3f& rDest) const;

        void multTranspose(const TVec3f& a1, const TVec3f& a2) const;

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

        void getXDir(TVec3f& rDest) const NO_INLINE {
            rDest.set(this->mMtx[0][0], this->mMtx[1][0], this->mMtx[2][0]);
        };

        void getYDir(TVec3f& rDest) const NO_INLINE {
            rDest.set(this->mMtx[0][1], this->mMtx[1][1], this->mMtx[2][1]);
        };

        void getZDir(TVec3f& rDest) const NO_INLINE {
            rDest.set(this->mMtx[0][2], this->mMtx[1][2], this->mMtx[2][2]);
        };

        void getXYZDir(TVec3f& rDestX, TVec3f& rDestY, TVec3f& rDestZ) const NO_INLINE {
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
        void setXDir(const TVec3f& rSrc);
        void setXDir(f32 x, f32 y, f32 z);

        inline void setXDirInline(f32 x, f32 y, f32 z) {
            this->mMtx[0][0] = x;
            this->mMtx[1][0] = y;
            this->mMtx[2][0] = z;
        }
        inline void setXDirInline(const TVec3f& rSrc) {
            this->mMtx[0][0] = rSrc.x;
            this->mMtx[1][0] = rSrc.y;
            this->mMtx[2][0] = rSrc.z;
        }

        inline void setYDirInline(f32 x, f32 y, f32 z) {
            this->mMtx[0][1] = x;
            this->mMtx[1][1] = y;
            this->mMtx[2][1] = z;
        }
        inline void setYDirInline(const TVec3f& rSrc) {
            this->mMtx[0][1] = rSrc.x;
            this->mMtx[1][1] = rSrc.y;
            this->mMtx[2][1] = rSrc.z;
        }

        inline void setZDirInline(f32 x, f32 y, f32 z) {
            this->mMtx[0][2] = x;
            this->mMtx[1][2] = y;
            this->mMtx[2][2] = z;
        }
        inline void setZDirInline(const TVec3f& rSrc) {
            this->mMtx[0][2] = rSrc.x;
            this->mMtx[1][2] = rSrc.y;
            this->mMtx[2][2] = rSrc.z;
        }

        void setYDir(const TVec3f& rSrc);
        void setYDir(f32 x, f32 y, f32 z);
        void setZDir(const TVec3f& rSrc);
        void setZDir(f32 x, f32 y, f32 z);
        void setXYZDir(const TVec3f& rSrcX, const TVec3f& rSrcY, const TVec3f& rSrcZ) NO_INLINE {
            setXYZDirInline(rSrcX, rSrcY, rSrcZ);
        }

        void getEuler(TVec3f& rDest) const {
            getEulerXYZ(rDest);
        }

        void getEulerXYZ(TVec3f& rDest) const {
            if (this->mMtx[2][0] - 1.0f >= -(f32)JGeometry::TUtil< f32 >::epsilon()) {
                rDest.set(JMAATan2(-this->mMtx[0][1], this->mMtx[1][1]), -HALF_PI, 0.0f);
                return;
            }

            f32 f1 = 1.0f;  // TODO: this is a hack to fix the float regswap
            if (this->mMtx[2][0] + f1 <= (f32)JGeometry::TUtil< f32 >::epsilon()) {
                rDest.set(JMAATan2(this->mMtx[0][1], this->mMtx[1][1]), HALF_PI, 0.0f);
                return;
            }

            rDest.x = JMAATan2(this->mMtx[2][1], this->mMtx[2][2]);
            rDest.z = JMAATan2(this->mMtx[1][0], this->mMtx[0][0]);
            rDest.y = JGeometry::TUtil< f32 >::asin(-this->mMtx[2][0]);
        }

        void setEulerX(f32 angle) {
            f32 s = sin(angle);
            f32 c = cos(angle);

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
            f32 s = sin(angle);
            f32 c = cos(angle);

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
            f32 s = sin(angle);
            f32 c = cos(angle);

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

        void getQuat(TQuat4f& rDest) const;
        void setQuat(const TQuat4f& q) {
            f32 yy = 2.0f * q.y * q.y;
            f32 zz = 2.0f * q.z * q.z;
            f32 xx = 2.0f * q.x * q.x;

            f32 xy = 2.0f * q.x * q.y;
            f32 xz = 2.0f * q.x * q.z;
            f32 yz = 2.0f * q.y * q.z;

            f32 wx = 2.0f * q.w * q.x;
            f32 wy = 2.0f * q.w * q.y;
            f32 wz = 2.0f * q.w * q.z;

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

        inline void fromQuat(const TQuat4f& q) {
            // in cases where setQuat doesnt quite work
            q.makeMtx((MtxPtr)this);
        }

        void getScale(TVec3f& rDest) const;
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

            // MATCHES {
            //    TPos3f::makeRotate
            //    OceanSphere definition
            //    DiskGravity::updateLocalParam
            //    OceanRingPipe::initPoints
            //    SegmentGravity::updateLocalParam
            //    SurfRay::updateRotate
            //    Plant::initLeaf
            // }
            //
            // DOES NOT MATCH {
            // all of these have the exact same regswap pattern
            //    DummyDisplayModel::calcAndSetBaseMtx (regswap)
            //    HomingKiller::calcAndSetBaseMtx (regswap)
            //    PowerStar::calcAndSetBaseMtx (regswap)
            //    BossBegomanHead::calcJointEdge (regswap)
            //    CocoNutBall::setVelocityToPlayer (regswap)
            //    KoopaFigureBall::control (regswap)
            //    BeeFlowerHover::control (regswap)
            //    BreakableCage::calcAndSetBaseMtx (regswap)
            //    CocoNut::updateRotate (regswap)
            //    FirePressureRadiate::calcJointCannon (regswap)
            // }

            f32 s = sin(angle);
            f32 c = cos(angle);

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

        void setRotate(const TVec3f& v1, const TVec3f& v2) {
            TQuat4f q;
            q.setRotate(v1, v2);
            setQuat(q);
        }

        void setRotate(const TVec3f& rRot) {
            setRotate(rRot.x, rRot.y, rRot.z);
        }

        void setRotate(f32 rx, f32 ry, f32 rz) {
            // NOTE: setEulerXYZ?
            f32 sinX, sinY, sinZ;
            f32 cosX, cosY, cosZ;

            cosZ = cos(rz);
            cosY = cos(ry);
            cosX = cos(rx);
            sinZ = sin(rz);
            sinY = sin(ry);
            sinX = sin(rx);

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

        void mult33(TVec3f&) const;
        void mult33(const TVec3f& rDst, TVec3f& rSrc) const;

        inline void getXDirInline(TVec3f& rDest) const {
            f32 z = this->mMtx[2][0];
            f32 y = this->mMtx[1][0];
            f32 x = this->mMtx[0][0];
            rDest.set< f32 >(x, y, z);
        }

        inline void getYDirInline(TVec3f& rDest) const {
            f32 z = this->mMtx[2][1];
            f32 y = this->mMtx[1][1];
            f32 x = this->mMtx[0][1];
            rDest.set< f32 >(x, y, z);
        }
        inline void getYDirInline2(TVec3f& rDest) const {
            f32 x = this->mMtx[0][1];
            f32 y = this->mMtx[1][1];
            f32 z = this->mMtx[2][1];
            rDest.set< f32 >(x, y, z);
        }

        inline void getZDirInline(TVec3f& rDest) const {
            f32 z = this->mMtx[2][2];
            f32 y = this->mMtx[1][2];
            f32 x = this->mMtx[0][2];
            rDest.set< f32 >(x, y, z);
        }

        inline void setXYZDirInline(const TVec3f& rSrcX, const TVec3f& rSrcY, const TVec3f& rSrcZ) {
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

        inline void setXYZDirInline2(const TVec3f& rSrcX, const TVec3f& rSrcY, const TVec3f& rSrcZ) {
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

        inline void getXYZDirInline(TVec3f& rDstX, TVec3f& rDstY, TVec3f& rDstZ) {
            f32 z1 = this->mMtx[2][0];
            f32 y1 = this->mMtx[1][0];
            f32 x1 = this->mMtx[0][0];
            rDstX.set< f32 >(x1, y1, z1);

            f32 z2 = this->mMtx[2][1];
            f32 y2 = this->mMtx[1][1];
            f32 x2 = this->mMtx[0][1];
            rDstY.set< f32 >(x2, y2, z2);

            f32 z3 = this->mMtx[2][2];
            f32 y3 = this->mMtx[1][2];
            f32 x3 = this->mMtx[0][2];
            rDstZ.set< f32 >(x3, y3, z3);
        }

        inline void mult33Inline(const TVec3f& rSrc, TVec3f& rDst) const {
            f32 a32, a22, a12, a11, a21, vx, a31, vy, a23, a33, a13;
            a32 = this->mMtx[2][1];
            a22 = this->mMtx[1][1];
            a12 = this->mMtx[0][1];
            a31 = this->mMtx[2][0];
            a21 = this->mMtx[1][0];
            a11 = this->mMtx[0][0];
            a33 = this->mMtx[2][2];
            a13 = this->mMtx[0][2];
            a23 = this->mMtx[1][2];
            f32 x, y;
            vx = rSrc.x;
            vy = rSrc.y;
            x = (vx * a11 + vy * a12);
            rDst.set< f32 >(rSrc.z * a13 + (vx * a11 + vy * a12), rSrc.z * a23 + (vx * a21 + vy * a22), rSrc.z * a33 + (rSrc.x * a31 + rSrc.y * a32));
        }
    };

    template < class T >
    struct TPosition3 : public TRotation3< T > {
    public:
        TPosition3(){};

        TPosition3(MtxPtr rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        TPosition3(const TPosition3< T >* rSrc) {
            JMath::gekko_ps_copy12(this, rSrc);
        }

        void getTrans(TVec3f& rDest) const;

        void setTrans(const TVec3f& rSrc) {
            this->mMtx[0][3] = rSrc.x;
            this->mMtx[1][3] = rSrc.y;
            this->mMtx[2][3] = rSrc.z;
        }

        void setTrans(f32 x, f32 y, f32 z);

        void zeroTrans() {
            this->mMtx[0][3] = 0.0f;
            this->mMtx[1][3] = 0.0f;
            this->mMtx[2][3] = 0.0f;
        }

        void makeRotate(const TVec3f& rVec, f32 angle) {
            zeroTrans();
            TRotation3< T >::setRotate(rVec, angle);
        }

        void makeQuat(const TQuat4f& rSrcQuat) {
            zeroTrans();
            TRotation3< T >::setQuat(rSrcQuat);
        }

        inline void makeQuatInline(const TQuat4f& rSrcQuat) {
            zeroTransInline();
            TRotation3< T >::setQuat(rSrcQuat);
        }

        void setPositionFromLookAt(const TPosition3< T >& rLookAt);

        void setQT(const TQuat4f& rSrcQuat, const TVec3f& rSrcTrans) {
            TRotation3< T >::setQuat(rSrcQuat);
            setTrans(rSrcTrans);
        }

        void setRT(f32 rx, f32 ry, f32 rz, const TVec3f& rSrcTrans) {
            // nonmatching, see
            // see Mogucchi::updateReferenceMtx

            f32 sinX, sinY, sinZ;
            f32 cosX, cosY, cosZ;

            cosZ = cos(rz);
            cosY = cos(ry);
            cosX = cos(rx);
            sinZ = sin(rz);
            sinY = sin(ry);
            sinX = sin(rx);

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

        inline void getTransInline(TVec3f& rDest) const {
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

        inline void setVec(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3) {
            this->mMtx[0][0] = a1.x;
            this->mMtx[1][0] = a1.y;
            this->mMtx[2][0] = a1.z;

            this->mMtx[0][1] = a2.x;
            this->mMtx[1][1] = a2.y;
            this->mMtx[2][1] = a2.z;

            this->mMtx[0][2] = a3.x;
            this->mMtx[1][2] = a3.y;
            this->mMtx[2][2] = a3.z;
        }

        inline void setPos(const TVec3f& a1) {
            this->mMtx[0][3] = a1.x;
            this->mMtx[1][3] = a1.y;
            this->mMtx[2][3] = a1.z;
        }

        inline void setTransInline(const TVec3f& a1) {
            this->mMtx[0][3] = a1.x;
            this->mMtx[1][3] = a1.y;
            this->mMtx[2][3] = a1.z;
        }

        inline void setTransInline(f32 x, f32 y, f32 z) {
            this->mMtx[0][3] = x;
            this->mMtx[1][3] = y;
            this->mMtx[2][3] = z;
        }

        inline void zeroTransInline() {
            this->mMtx[0][3] = 0.0f;
            this->mMtx[1][3] = 0.0f;
            this->mMtx[2][3] = 0.0f;
        }

        inline void zeroTransInline2() {
            this->mMtx[0][3] = 0.0f;
            this->mMtx[1][3] = 0.0f;
            this->mMtx[2][3] = 0.0f;
        }

        inline void setVecAndTransInline(const TVec3f& a1, const TVec3f& a2, const TVec3f& a3, const TVec3f& a4) {
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

        inline void normalizeBasis() {
            f32 scale = JGeometry::TUtil< f32 >::sqrt(
                (this->mMtx[0][0] * this->mMtx[0][0]) + (this->mMtx[1][0] * this->mMtx[1][0]) + (this->mMtx[2][0] * this->mMtx[2][0]) +
                (this->mMtx[0][1] * this->mMtx[0][1]) + (this->mMtx[1][1] * this->mMtx[1][1]) + (this->mMtx[2][1] * this->mMtx[2][1]) +
                (this->mMtx[0][2] * this->mMtx[0][2]) + (this->mMtx[1][2] * this->mMtx[1][2]) + (this->mMtx[2][2] * this->mMtx[2][2]));

            f32 invLenX = JGeometry::TUtil< f32 >::inv_sqrt((this->mMtx[0][0] * this->mMtx[0][0]) + (this->mMtx[1][0] * this->mMtx[1][0]) +
                                                            (this->mMtx[2][0] * this->mMtx[2][0]));

            this->mMtx[0][0] = invLenX * this->mMtx[0][0];
            this->mMtx[1][0] = invLenX * this->mMtx[1][0];
            this->mMtx[2][0] = invLenX * this->mMtx[2][0];

            f32 invLenY = JGeometry::TUtil< f32 >::inv_sqrt((this->mMtx[0][1] * this->mMtx[0][1]) + (this->mMtx[1][1] * this->mMtx[1][1]) +
                                                            (this->mMtx[2][1] * this->mMtx[2][1]));

            this->mMtx[0][1] = invLenY * this->mMtx[0][1];
            this->mMtx[1][1] = invLenY * this->mMtx[1][1];
            this->mMtx[2][1] = invLenY * this->mMtx[2][1];

            f32 invLenZ = JGeometry::TUtil< f32 >::inv_sqrt((this->mMtx[0][2] * this->mMtx[0][2]) + (this->mMtx[1][2] * this->mMtx[1][2]) +
                                                            (this->mMtx[2][2] * this->mMtx[2][2]));

            this->mMtx[0][2] = invLenZ * this->mMtx[0][2];
            this->mMtx[1][2] = invLenZ * this->mMtx[1][2];
            this->mMtx[2][2] = invLenZ * this->mMtx[2][2];
        }
    };

    template < typename T >
    struct SMatrix44C {
    public:
        typedef f32 ArrType[4];
        void set(const ArrType*);
        void set(const SMatrix44C< T >& rSrc);
        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz, T wx, T wy, T wz, T ww);

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
        void identity();
        void concat(const T& rSrcA, const T& rSrcB);
        void concat(const T& rSrc);
        void invert(const TMatrix44< T >& rDest);

        inline void mult(const TVec3f& rSrc, TVec3f& rDest) const {
            TVec4f pos(rSrc.x * this->mMtx[0][0] + rSrc.z * this->mMtx[0][2], rSrc.y * this->mMtx[1][1] + rSrc.z * this->mMtx[1][2],
                       rSrc.z * this->mMtx[2][2] + this->mMtx[2][3], -rSrc.z);

            rDest.scale(1.0f / pos.w, *pos.toTVec3());
        }
    };

    template < class T >
    struct TProjection3 : public T {
    public:
        TProjection3(){};

        TProjection3(const Mtx44Ptr rSrc) {
            JMath::gekko_ps_copy16(this, rSrc);
        }

        void identity44();
    };

};  // namespace JGeometry

typedef JGeometry::SMatrix34C< f32 > TSMtxf;
typedef JGeometry::TMatrix34< TSMtxf > TMtx34f;
typedef JGeometry::TRotation3< TMtx34f > TRot3f;
typedef JGeometry::TPosition3< TMtx34f > TPos3f;
typedef JGeometry::SMatrix44C< f32 > TSMtx44f;
typedef JGeometry::TMatrix44< TSMtx44f > TMtx44f;
typedef JGeometry::TProjection3< TMtx44f > TProj3f;

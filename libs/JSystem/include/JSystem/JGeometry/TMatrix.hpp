#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "JSystem/JGeometry/TQuat.hpp"
#include <cmath>

namespace JGeometry {
    template<typename T>
    struct SMatrix34C {
    public:
        void set(const MtxPtr);
        void set(const SMatrix34C<T> &rSrc);
        void set(T rxx, T ryx, T rzx, T tx, T rxy, T ryy, T rzy, T ty, T rxz, T ryz, T rzz, T tz);

        void scale(T);

        inline void setInline(const SMatrix34C<T> &rSrc) {
            register const SMatrix34C<T> *pSrc = &rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            };
        }

        inline void setInline(const MtxPtr rSrc) {
            register const MtxPtr pSrc = rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            };
        }

        inline void setInline_2(MtxPtr rSrc) {
            register MtxPtr pSrc = rSrc;
            register SMatrix34C<T> *pDest = this;
            register f32 rzztz;
            register f32 rxzyz;
            register f32 rzyty;
            register f32 rxyyy;
            register f32 rzxtx;
            register f32 rxxyx;

            __asm {
                psq_l     rxxyx, 0(pSrc), 0, 0
                psq_l     rzxtx, 8(pSrc), 0, 0
                psq_l     rxyyy, 0x10(pSrc), 0, 0
                psq_st    rxxyx, 0(pDest), 0, 0
                psq_l     rzyty, 0x18(pSrc), 0, 0
                psq_st    rzxtx, 8(pDest), 0, 0
                psq_l     rxzyz, 0x20(pSrc), 0, 0
                psq_st    rxyyy, 0x10(pDest), 0, 0
                psq_l     rzztz, 0x28(pSrc), 0, 0
                psq_st    rzyty, 0x18(pDest), 0, 0
                psq_st    rxzyz, 0x20(pDest), 0, 0
                psq_st    rzztz, 0x28(pDest), 0, 0
            };
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

        T mMtx[3][4];
        typedef T type;
    };

    template<typename T>
    struct TMatrix34 : public T {
    public:
        void identity();
        void concat(const T &rSrcA, const T &rSrcB);
        void concat(const T &rSrc);
        void invert(const TMatrix34<T> &rDest);

        void mult(const TVec3f &rSrc, TVec3f &rDest) const;

        void multTranspose(const TVec3f &a1, const TVec3f &a2) const;
        typedef T::type type;
    };
    template<class T>
    struct TRotation3 : public T {
    public:
    typedef T::type type;
        void identity33();

        void getXDir(TVec3f &rDest) const NO_INLINE {
            rDest.set<f32>(mMtx[0][0], mMtx[1][0], mMtx[2][0]);
        };

        void getYDir(TVec3f &rDest) const NO_INLINE {
            rDest.set<f32>(mMtx[0][1], mMtx[1][1], mMtx[2][1]);
        };

        void getZDir(TVec3f &rDest) const NO_INLINE {
            rDest.set<f32>(mMtx[0][2], mMtx[1][2], mMtx[2][2]);
        };

        void getXYZDir(TVec3f &rDestX, TVec3f &rDestY, TVec3f &rDestZ) const;
        void setXDir(const TVec3f &rSrc);
        void setXDir(f32 x, f32 y, f32 z);
        void setYDir(const TVec3f &rSrc);
        void setYDir(f32 x, f32 y, f32 z);
        void setZDir(const TVec3f &rSrc);
        void setZDir(f32 x, f32 y, f32 z);
        void setXYZDir(const TVec3f &rSrcX, const TVec3f &rSrcY, const TVec3f &rSrcZ);

        void getEuler(TVec3f &rDest) const;
        void getEulerXYZ(TVec3f &rDest) const;
        void setEulerY(f32 val);
        void setEulerZ(f32 val);

        void getQuat(TQuat4f &rDest) const;
        void setQuat(const TQuat4f &rSrc);

        void getScale(TVec3f &rDest) const;
        void setScale(const TVec3f &rSrc);


        /*void helper(const TVec3<type> &stack_8, type fr1ex, type fr1ey) {
            
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;  
            mMtx[0][0] = fr1ex + (1.0f - fr1ex) * (stack_8.x * stack_8.x);
            mMtx[0][1] = (1.0f - fr1ex) * stack_8.x * stack_8.y - fr1ey * stack_8.z;
            mMtx[0][2] = (1.0f - fr1ex) * stack_8.x * stack_8.z + fr1ey * stack_8.y;
            mMtx[1][0] = (1.0f - fr1ex) * stack_8.x * stack_8.y + fr1ey * stack_8.z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * (stack_8.y * stack_8.y);
            mMtx[1][2] = (1.0f - fr1ex) * stack_8.y * stack_8.z - fr1ey * stack_8.x;
            mMtx[2][0] = (1.0f - fr1ex) * stack_8.x * stack_8.z - fr1ey * stack_8.y;
            mMtx[2][1] = (1.0f - fr1ex) * stack_8.y * stack_8.z + fr1ey * stack_8.x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * (stack_8.z * stack_8.z);

        }
        void setRotate(const TVec3f &, const TVec3f &);
        void setRotate(TVec3<type> &mLocalDirection, type fr1e) NO_INLINE {
            TVec3<type> stack_8;
            stack_8.set(mLocalDirection);
            PSVECMag(stack_8.toCVec());
            PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
            //f32 fr1ey = , fr1ex = ;
            helper(stack_8, cos(fr1e), sin(fr1e));
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            
        }*/
        void setRotate(const TVec3f &, f32);
        inline void help(f32 x, f32 y, f32 z, f32 fr1ex, f32 fr1ey) {
            mMtx[0][0] = fr1ex + (1.0f - fr1ex) * yy(x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * yy(y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * yy(z);
        }
        inline void multP(TVec3<type> &v) const {
            T::mult(v, v);
        }
        void INLINE_FUNC_DECL(setRotate, const TVec3f &mLocalDirection, f32 fr1e) {
            //union {TVec3f stack_81; const Vec stack_8;};
            //stack_81.setInline(mLocalDirection);
            //const TVec3f &stack_8 = *(const TVec3f*)&bs;
            TVec3f stack_8;
            stack_8.set(mLocalDirection);
            PSVECMag(stack_8.toCVec());
            PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
            //const TVec3f stack_8 = stack_8;
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            //f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
            f32 x, y, z;
            //f32 &rx = x, &ry = y, &rz = z;
            x = stack_8.x;
            y = stack_8.y;
            z = stack_8.z;
            f32 xx = x * x;
            f32 yx = y * y;
            f32 zz = z * z;
            //f32 yy = y * y;
            //f32 diff = 1.0f - fr1ex;

            mMtx[0][0] = fr1ex + (1.0f - fr1ex) * (x * x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * (y * y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * (z * z);
            
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;  
            /*mMtx[0][0] = fr1ex + (1.0f - fr1ex) * yy(x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * yy(y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * yy(z);*/
        }

        void setRotateInline2(const TVec3f &mLocalDirection, f32 fr1e) {
            //union {TVec3f stack_81; const Vec stack_8;};
            //stack_81.setInline(mLocalDirection);
            //const TVec3f &stack_8 = *(const TVec3f*)&bs;
            TVec3f stack_8;
            // We can just accept that nobody wants to go through the effort of controlling
            // when this does/does not get inlined
            stack_8.setInline(mLocalDirection);
            PSVECMag(stack_8.toCVec());
            PSVECNormalize(stack_8.toCVec(), stack_8.toVec());
            //const TVec3f stack_8 = stack_8;
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
        
            //f32 fr1ex, fr1ey;
        
            // 8 reg -- 3 for stack_8, 3 for calculation, 1 for fr1ex, 1 for 1.0f - fr1ex
            //f32 fr1ey = sin(fr1e), fr1ex = cos(fr1e);
            f32 x, y, z;
            //f32 &rx = x, &ry = y, &rz = z;
            x = stack_8.x;
            y = stack_8.y;
            z = stack_8.z;
            f32 xx = x * x;
            f32 yx = y * y;
            f32 zz = z * z;
            //f32 yy = y * y;
            //f32 diff = 1.0f - fr1ex;

            mMtx[0][0] = fr1ex + (1.0f - fr1ex) * (x * x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * (y * y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * (z * z);
            
            //f32 (1.0f - fr1ex) = 1.0f - fr1ex;  
            /*mMtx[0][0] = fr1ex + (1.0f - fr1ex) * yy(x);
            mMtx[0][1] = (1.0f - fr1ex) * x * y - fr1ey * z;
            mMtx[0][2] = (1.0f - fr1ex) * x * z + fr1ey * y;
            mMtx[1][0] = (1.0f - fr1ex) * x * y + fr1ey * z;
            mMtx[1][1] = fr1ex + (1.0f - fr1ex) * yy(y);
            mMtx[1][2] = (1.0f - fr1ex) * y * z - fr1ey * x;
            mMtx[2][0] = (1.0f - fr1ex) * x * z - fr1ey * y;
            mMtx[2][1] = (1.0f - fr1ex) * y * z + fr1ey * x;
            mMtx[2][2] = fr1ex + (1.0f - fr1ex) * yy(z);*/
        }

        void mult33(TVec3f &) const;
        void mult33(const TVec3f &rDst, TVec3f &rSrc) const;

        inline void getXDirInline(TVec3f &rDest) const {
            f32 z = mMtx[2][0];
            f32 y = mMtx[1][0];
            f32 x = mMtx[0][0];
            rDest.set(x, y, z);
        }

        inline f32 helpA(const TVec3f &rSrc) const {
            return rSrc.z * mMtx[0][2] + rSrc.x * mMtx[0][0] + rSrc.y * mMtx[0][1];
        }
        inline f32 helpB(const TVec3f &rSrc) const {
            return rSrc.z * mMtx[1][2] + rSrc.x * mMtx[1][0] + rSrc.y * mMtx[1][1];
        }
        inline f32 helpC(const TVec3f &rSrc) const {
            return rSrc.z * mMtx[2][2] + rSrc.x * mMtx[2][0] + rSrc.y * mMtx[2][1];
        }

        template<typename U>
        inline U sum(U a, U b) const {
            return a + b;
        }


void stupidM(const TVec3f &rSrc, f32 &x, f32 &y, f32 &z) const {
            x = rSrc.z * mMtx[0][2] + rSrc.x * mMtx[0][0] + rSrc.y * mMtx[0][1];
             y = rSrc.z * mMtx[1][2] + rSrc.x * mMtx[1][0] + rSrc.y * mMtx[1][1];
             z = rSrc.z * mMtx[2][2] + rSrc.x * mMtx[2][0] + rSrc.y * mMtx[2][1];
}

        inline void math(f32 &dstX, f32 &dstY, f32 &dstZ, const f32 &srcX, const f32 &srcY) const {
            dstZ = addd(mMtx[2][0] * srcX , mMtx[2][1] * srcY);
            dstY = addd(mMtx[1][0] * srcX , mMtx[1][1] * srcY);
            dstX = addd(mMtx[0][0] * srcX , mMtx[0][1] * srcY);
            
        }
        inline void mult33Inline(const TVec3f &rSrc, TVec3f &rDst) const {
            /*f32 z, y, x;
            z = rSrc.x * mMtx[2][0];
            y = rSrc.x * mMtx[1][0];
            x = rSrc.x * mMtx[0][0];
            f32 tmp = mMtx[2][1];
            z += rSrc.y * tmp;
            y = rSrc.y * mMtx[1][1] + y;
            x = rSrc.y * mMtx[0][1] + x;
            x = rSrc.z * mMtx[0][2] + x;
            y = rSrc.z * mMtx[1][2] + y;
            z += rSrc.z * mMtx[2][2];*/
            //f32 a21;
            //f32 &a21p = a21;
            //f32 x, y, z;
            //f32 vx, vy, vz, z, y, x, a31, a21, a11;
            //f32 vx, vy, vz;
            f32 a32, a22, a12, a11, a21, vx, a31, vy, a23, a33, a13;
            a32 = mMtx[2][1];
            //vy = rSrc.y;
            
            a22 = mMtx[1][1];
            a12 = mMtx[0][1];
            a31 = mMtx[2][0];
            a21 = mMtx[1][0];
            a11 = mMtx[0][0];
            
            a33 = mMtx[2][2];
            a13 = mMtx[0][2];
            a23 = mMtx[1][2];
        
        
            //vx = rSrc.x;
        
            //f32 z, y, x;
        
            //z = vx * a31 + vy * a32;
            
            //y = vx * a21 + vy * a22;
            
            //x = vx * a11 + vy * a12;
        
            /*f32 z = rSrc.x * a31;
            f32 y = rSrc.x * a21;
            f32 x = rSrc.x * a11;*/
            
        
            
        
         
            //x = vz * a13 + x;
            //y = vz * a23 + y;
            //z = vz * a33 + z;
            //x += vz * a13;
            //y += vz * a23;
            //z += vz * a33;
        
            /*f32 z2 = z + rSrc.z * a33;
            f32 y2 = y + rSrc.z * a23;
            f32 x2 = x + rSrc.z * a13;*/
            //x = vx * a11 + vy * a12 + vz * a13;
            //y = vx * a21 + vy * a22 + vz * a23;
            //z = vx * a31 + vy * a32 + vz * a33;
            f32 x, y;
            vx = rSrc.x;
            vy = rSrc.y;
            //y = (vx * a21 + vy * a22);
            x = (vx * a11 + vy * a12);
            rDst.set(rSrc.z * a13 + (vx * a11 + vy * a12), rSrc.z * a23 + (vx * a21 + vy * a22), rSrc.z * a33 + (rSrc.x * a31 + rSrc.y * a32));
        }
    };

    inline f32 dot(f32 x, f32 y, f32 z, f32 x2, f32 y2, f32 z2) {
        return x2 * x + y2 * y + z2 * z;
    }

inline f32 yy(f32 y) {
    return y * y;
}
inline f32 g(f32 x) {
    return cos(x);
}
    
        inline f32 helper(const Vec &rSrc, const Vec &mtx) {
            return rSrc.y * mtx.y + rSrc.x  * mtx.x + rSrc.z * mtx.z;
        }

    template<class T>
    struct TPosition3 : public TRotation3<T> {
    public:
        void getTrans(TVec3f &rDest) const;
        void setTrans(const TVec3f &rSrc);
        void setTrans(f32 x, f32 y, f32 z);
        void zeroTrans();

        void makeRotate(const TVec3f &, f32);
        void makeQuat(const TQuat4f &rSrc);
        void setPositionFromLookAt(const TPosition3<T> &rLookAt);
        void setQT(const TQuat4f &rSrcQuat, const TVec3f &rSrcTrans);

        inline void getTransInline(TVec3f &rDest) const {
            f32 z = mMtx[2][3];
            f32 y = mMtx[1][3];
            f32 x = mMtx[0][3];
            rDest.set(x, y, z);
        }
    };
};

typedef JGeometry::SMatrix34C<f32> TSMtxf;
typedef JGeometry::TMatrix34<TSMtxf> TMtx34f;
typedef JGeometry::TRotation3<TMtx34f> TRot3f;
typedef JGeometry::TPosition3<TMtx34f> TPos3f;

#pragma once

#include "Game/Camera/CameraTargetObj.h"
#include "JSystem/JGeometry/TMatrix.h"

class CameraTargetMtx : public CameraTargetObj {
public:
    CameraTargetMtx(const char *);
    virtual ~CameraTargetMtx();
    
    virtual void movement();

    virtual const TVec3f *getPosition() const;
    virtual const TVec3f *getUpVec() const;
    virtual const TVec3f *getFrontVec() const;
    virtual const TVec3f *getSideVec() const;
    virtual const TVec3f *getLastMove() const;
    virtual const TVec3f *getGroundPos() const;
    virtual const TVec3f *getGravityVector() const;

    virtual CubeCameraArea *getCubeCameraArea() const;
    
    void invalidateLastMove();

    void setMtx(register MtxPtr mtx) NO_INLINE {
        register TMtx34f &dst = mMatrix;

        __asm {
            psq_l   f0, 0x00(mtx), 0x0, 0
            psq_l   f1, 0x08(mtx), 0x0, 0
            psq_l   f2, 0x10(mtx), 0x0, 0
            psq_l   f3, 0x18(mtx), 0x0, 0
            psq_l   f4, 0x20(mtx), 0x0, 0
            psq_l   f5, 0x28(mtx), 0x0, 0
            psq_st  f0, 0x00(dst), 0x0, 0
            psq_st  f1, 0x08(dst), 0x0, 0
            psq_st  f2, 0x10(dst), 0x0, 0
            psq_st  f3, 0x18(dst), 0x0, 0
            psq_st  f4, 0x20(dst), 0x0, 0
            psq_st  f5, 0x28(dst), 0x0, 0
        }
    }

    TMtx34f mMatrix;                // _10
    TVec3f mPosition;               // _40
    TVec3f mLastMove;               // _4C
    TVec3f mGravityVector;          // _58
    TVec3f mUp;                     // _64
    TVec3f mFront;                  // _70
    TVec3f mSide;                   // _7C
    bool mInvalidLastMove;          // _88
    u8 _89[3];
    CubeCameraArea *mCameraArea;    // _8C
};
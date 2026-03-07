#pragma once

#include "Game/Camera/CameraTargetObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraTargetMtx : public CameraTargetObj {
public:
    CameraTargetMtx(const char*);
    virtual ~CameraTargetMtx();

    virtual void movement();

    virtual const TVec3f* getPosition() const;
    virtual const TVec3f* getUpVec() const;
    virtual const TVec3f* getFrontVec() const;
    virtual const TVec3f* getSideVec() const;
    virtual const TVec3f* getLastMove() const;
    virtual const TVec3f* getGroundPos() const;
    virtual const TVec3f* getGravityVector() const;

    virtual CubeCameraArea* getCubeCameraArea() const;

    void invalidateLastMove();

    void setMtx(register MtxPtr mtx) NO_INLINE {
        register TMtx34f& dst = mMatrix;

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

    /* 0x10 */ TPos3f mMatrix;
    /* 0x40 */ TVec3f mPosition;
    /* 0x4C */ TVec3f mLastMove;
    /* 0x58 */ TVec3f mGravityVector;
    /* 0x64 */ TVec3f mUp;
    /* 0x70 */ TVec3f mFront;
    /* 0x7C */ TVec3f mSide;
    /* 0x88 */ bool mInvalidLastMove;
    /* 0x89 */ u8 _89[3];
    /* 0x8C */ CubeCameraArea* mCameraArea;
};

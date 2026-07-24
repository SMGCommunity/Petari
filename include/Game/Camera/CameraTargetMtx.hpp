#pragma once

#include "Game/Camera/CameraTargetObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraTargetMtx : public CameraTargetObj {
public:
    CameraTargetMtx(const char*);
    virtual ~CameraTargetMtx();

    virtual void movement();

    virtual const TVec3f& getPosition() const;
    virtual const TVec3f& getUpVec() const;
    virtual const TVec3f& getFrontVec() const;
    virtual const TVec3f& getSideVec() const;
    virtual const TVec3f& getLastMove() const;
    virtual const TVec3f& getGroundPos() const;
    virtual const TVec3f& getGravityVector() const;

    virtual CubeCameraArea* getCubeCameraArea() const;

    void invalidateLastMove();

    void setMtx(register MtxPtr mtx) {
        mMatrix.set(mtx);
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

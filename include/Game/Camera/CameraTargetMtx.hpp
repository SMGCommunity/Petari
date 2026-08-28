#pragma once

#include "Game/Camera/CameraTargetObj.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"

class CameraTargetMtx : public CameraTargetObj {
public:
    CameraTargetMtx(const char*);
    /* 0x08 */ virtual ~CameraTargetMtx();

    /* 0x14 */ virtual void movement();

    /* 0x24 */ virtual const TVec3f& getPosition() const;
    /* 0x28 */ virtual const TVec3f& getUpVec() const;
    /* 0x2C */ virtual const TVec3f& getFrontVec() const;
    /* 0x30 */ virtual const TVec3f& getSideVec() const;
    /* 0x34 */ virtual const TVec3f& getLastMove() const;
    /* 0x38 */ virtual const TVec3f& getGroundPos() const;
    /* 0x3C */ virtual const TVec3f& getGravityVector() const;

    /* 0x68 */ virtual CubeCameraArea* getCubeCameraArea() const;

    void invalidateLastMove();

    void setMtx(MtxPtr mtx) {
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
    /* 0x8C */ CubeCameraArea* mCameraArea;
};

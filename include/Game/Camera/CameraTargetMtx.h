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
    void setMtx(MtxPtr);

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
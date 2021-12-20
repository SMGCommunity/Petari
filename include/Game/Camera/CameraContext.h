#pragma once

#include "Game/NameObj/NameObj.h"
#include "JSystem/JGeometry/TMatrix.h"
#include "JSystem/JGeometry/TPosition3.h"
#include "JSystem/JGeometry/TVec.h"

class CameraContext : public NameObj {
public:
    CameraContext();
    virtual ~CameraContext();

    void initParams();
    const TMtx34f *getViewMtx() const;
    const TMtx34f *getInvViewMtx() const;
    //getAspect() const;
    //setViewMtx(const TPos3f &, bool, bool, const TVec3f &);
    void setNearZ(float);
    void setFovy(float);
    void setShakeOffset(float, float);
    void updateProjectionMtx();

    TMtx34f mView;          // _C
    TMtx34f mViewInv;       // _3C
    TMtx34f mProjection;    // _6C
    f32 _9C;
    f32 _A0;
    f32 _A4;
    f32 _A8;
    f32 _AC;
    f32 mNearZ;             // _B0
    f32 mFarZ;              // _B4
    f32 mFovy;              // _B8
    f32 mShakeOffsetX;      // _BC
    f32 mShakeOffsetY;      // _C0
};
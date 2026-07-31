#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry.hpp"

class CameraContext : public NameObj {
public:
    CameraContext();
    virtual ~CameraContext();

    void initParams();
    const TPos3f* getViewMtx() const;
    const TPos3f* getInvViewMtx() const;
    f32 getAspect() const;
    void setViewMtx(const TPos3f&, bool, bool, const TVec3f&);
    void setNearZ(f32);
    void setFarZ(f32);
    void setFovy(f32);
    void setShakeOffset(f32, f32);
    void updateProjectionMtx();

    f32 getNearZ() const {
        return mNearZ;
    }

    f32 getFarZ() const {
        return mFarZ;
    }

    f32 getFovy() const {
        return mFovy;
    }

    /* 0x0C */ TPos3f mView;
    /* 0x3C */ TPos3f mViewInv;
    /* 0x6C */ TProj3f mProjection;
    /* 0xAC */ f32 mAspectRatio;
    /* 0xB0 */ f32 mNearZ;
    /* 0xB4 */ f32 mFarZ;
    /* 0xB8 */ f32 mFovy;
    /* 0xBC */ TVec2f mShakeOffset;
};

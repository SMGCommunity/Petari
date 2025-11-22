#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class DiskTorusGravity : public PlanetGravity {
public:
    DiskTorusGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setPosition(const TVec3f& rPosition);
    void setDirection(const TVec3f& rDirection);
    void setRadius(f32 val);
    void setDiskRadius(f32 val);
    void setEdgeType(s32 val);
    void setBothSide(bool val);

    TVec3f mLocalPosition;   // 0x28
    TVec3f mTranslation;     // 0x34
    TVec3f mLocalDirection;  // 0x40
    TVec3f mRotation;        // 0x4C
    f32 mRadius;             // 0x58
    f32 mWorldRadius;        // 0x5C
    f32 mDiskRadius;         // 0x60
    s32 mEdgeType;           // 0x64
    bool mEnableBothSide;    // 0x68
};

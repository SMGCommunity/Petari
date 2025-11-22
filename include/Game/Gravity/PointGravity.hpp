#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class PointGravity : public PlanetGravity {
public:
    PointGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    TVec3f mOrigPosition;  // 0x28
    TVec3f mTranslation;   // 0x34
};

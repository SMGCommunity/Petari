#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class PointGravity : public PlanetGravity {
public:
    PointGravity();

    /* 0x08 */ virtual void updateMtx(const TPos3f& rMtx);
    /* 0x0C */ virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setOrig(const TVec3f& rOrig) {
        mOrigPosition = rOrig;
    }

    TVec3f mOrigPosition;  // 0x28
    TVec3f mTranslation;   // 0x34
};

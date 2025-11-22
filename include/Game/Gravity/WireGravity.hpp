#pragma once

#include "Game/Gravity/PlanetGravity.hpp"
#include "Game/Util/Array.hpp"

class WireGravity : public PlanetGravity {
public:
    WireGravity();

    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setPointListSize(u32);
    void addPoint(const TVec3f&);

    MR::Vector< MR::AssignableArray< TVec3f > > mPoints;  // 0x28
};

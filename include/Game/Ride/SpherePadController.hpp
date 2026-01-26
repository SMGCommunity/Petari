#pragma once

#include "Game/Ride/SphereController.hpp"

class SpherePadController : SphereController {
public:
    SpherePadController();

    virtual f32 calcMoveVector(TVec3f*, const TVec3f&);
    virtual f32 calcJumpPower() const;
    virtual void update(const TVec3f&);
    virtual void clacXY(f32*, f32*) NO_INLINE;
    // virtual void genMessage(JORMContext*); // DEBUG

    f32 calcDirSphereMove(TVec3f*, const TVec3f&, u32);
};

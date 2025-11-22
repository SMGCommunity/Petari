#include "Game/Ride/SphereController.hpp"

SphereController::SphereController() :
    _4(0)
{
    
}

f32 SphereController::calcMoveVector(TVec3f* pDst, const TVec3f& rSrc) {
    return pDst->x = pDst->y = pDst->z = 0.0f;
}

void SphereController::update(const TVec3f& sSrc) {
    
}

bool SphereController::doBrake() const {
    return false;
}

f32 SphereController::calcJumpPower() const {
    return 0.0f;
}

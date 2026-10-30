#include "Game/Util/TriangleFilter.hpp"
#include "Game/Util/MapUtil.hpp"

TriangleFilterFunc* MR::createTriangleFilterFunc(TriangleFunc func) {
    return new TriangleFilterFunc(func);
}

bool TriangleFilterDangerCode::isInvalidTriangle(const Triangle* pTriangle) const {
    return MR::isGroundCodeDeath(pTriangle) || MR::isGroundCodeDamage(pTriangle) || MR::isGroundCodeDamageFire(pTriangle) ||
           MR::isGroundCodeFireDance(pTriangle) || MR::isGroundCodeDamageElectric(pTriangle) || MR::isGroundCodeSinkDeath(pTriangle) ||
           MR::isGroundCodeSinkDeathMud(pTriangle);
}

#include "Game/Util/TriangleFilter.hpp"
#include "Game/Util/MapUtil.hpp"

namespace MR {
    TriangleFilterFunc* createTriangleFilterFunc(TriangleFunc func) {
        return new TriangleFilterFunc(func);
    }
};  // namespace MR

bool TriangleFilterDangerCode::isInvalidTriangle(const Triangle* pTriangle) const {
    bool isInvalidTriangle = false;

    if (MR::isGroundCodeDeath(pTriangle) || MR::isGroundCodeDamage(pTriangle) || MR::isGroundCodeDamageFire(pTriangle) ||
        MR::isGroundCodeFireDance(pTriangle) || MR::isGroundCodeDamageElectric(pTriangle) || MR::isGroundCodeSinkDeath(pTriangle) ||
        MR::isGroundCodeSinkDeathMud(pTriangle)) {
        isInvalidTriangle = true;
    }

    return isInvalidTriangle;
}

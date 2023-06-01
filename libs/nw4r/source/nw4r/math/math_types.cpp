#include "nw4r/math/types.h"

namespace nw4r {
    namespace math {
        VEC3* VEC3TransformNormal(VEC3* pOut, const MTX34* pM, const VEC3* pV) {
            VEC3 tmp;
            tmp.x = pM->_00 * pV->x + pM->_01 * pV->y + pM->_02 * pV->z;
            tmp.y = pM->_10 * pV->x + pM->_11 * pV->y + pM->_12 * pV->z;
            tmp.z = pM->_20 * pV->x + pM->_21 * pV->y + pM->_22 * pV->z;

            pOut->x = tmp.x;
            pOut->y = tmp.y;
            pOut->z = tmp.z;
            return pOut;
        }
    };
};
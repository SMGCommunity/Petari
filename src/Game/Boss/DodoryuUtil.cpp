#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Util/MathUtil.hpp"

namespace DodoryuUtil {
    bool calcVerticalizedDir(Dodoryu* pDodoryu, TVec3f* pVec, const TVec3f& rVec) {
        TVec3f yDir;
        pDodoryu->mBaseMtx.getYDir(yDir);

        pVec->scaleAdd(yDir, rVec, -yDir.dot(rVec));

        if (MR::isNearZero(*pVec)) {
            return false;
        }

        MR::normalize(pVec);

        return true;
    }
};  // namespace DodoryuUtil

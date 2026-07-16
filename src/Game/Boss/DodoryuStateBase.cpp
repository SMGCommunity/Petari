#include "Game/Boss/DodoryuStateBase.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Util/MathUtil.hpp"

// DodoryuStateBase::calcAttackDir(TVec3f*, const TVec3f&, const TVec3f&)

bool DodoryuStateBase::calcVerticalizedDir(TVec3f* pVec, const TVec3f& rVec) {
    // TODO: Possibly an inline of `DodoryuUtil::calcVerticalizedDir`.
    TVec3f yDir;
    mHost->mBaseMtx.getYDir(yDir);

    pVec->killElement(rVec, yDir);

    if (MR::isNearZero(*pVec)) {
        return false;
    }

    MR::normalize(pVec);

    return true;
}

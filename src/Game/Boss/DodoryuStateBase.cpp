#include "Game/Boss/DodoryuStateBase.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void DodoryuStateBase::calcAttackDir(TVec3f* pVec, const TVec3f& rVec1, const TVec3f& rVec2) {
    pVec->sub(rVec2, rVec1);

    pVec->orthogonalize(mHost->mGravity);
    MR::normalizeOrZero(pVec);

    if (MR::isNearZero(*pVec)) {
        mHost->mBaseMtx.getXDir(*pVec);
    }

    TVec3f yDir;
    mHost->mBaseMtx.getYDir(yDir);
    pVec->add(yDir);
    MR::normalize(pVec);
}

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

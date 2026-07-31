#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Util/MathUtil.hpp"

namespace DodoryuUtil {
    bool calcVerticalizedDir(Dodoryu* pDodoryu, TVec3f* pVec, const TVec3f& rVec) {
        TVec3f yDir;
        pDodoryu->mBaseMtx.getYDir(yDir);

        pVec->killElement(rVec, yDir);

        if (MR::isNearZero(*pVec)) {
            return false;
        }

        MR::normalize(pVec);

        return true;
    }

    // turnToward

    void accelerate(Dodoryu* pDodoryu, const TVec3f& rVec, f32 param3, f32 param4, f32 param5) {
        TVec3f auStack_40;
        TVec3f auStack_34 = rVec;
        auStack_40.killElement(pDodoryu->mVelocity, rVec);
        pDodoryu->mVelocity -= auStack_40;
        auStack_40 *= param4;
        pDodoryu->mVelocity += auStack_40;
        pDodoryu->mVelocity += auStack_34 * param3;

        if (pDodoryu->mVelocity.length() > param5) {
            pDodoryu->mVelocity.setLength(param5);
        }

        TVec3f yDir;
        pDodoryu->mBaseMtx.getYDir(yDir);
        pDodoryu->mVelocity.orthogonalize(yDir);
    }

    void addVelocity(Dodoryu* pDodoryu, bool isSnap) {
        TVec3f trans;
        TPos3f mtx = pDodoryu->mBaseMtx;

        mtx.getTrans(trans);
        trans.add(pDodoryu->mVelocity);
        mtx.setTrans(trans);
        pDodoryu->setMtx(mtx);

        if (isSnap) {
            pDodoryu->snapToGround();
        }
    }

    void rotateVelocityByWall(Dodoryu* pDodoryu) {
        TVec3f auStack_2c;
        pDodoryu->mBaseMtx.getYDir(auStack_2c);

        f32 speed = pDodoryu->mVelocity.length();
        TVec3f auStack_38 = pDodoryu->mVelocity;
        auStack_38.orthogonalize(pDodoryu->_134);

        if (MR::isNearZero(auStack_38)) {
            auStack_38.cross(auStack_2c, pDodoryu->_134);
        }

        auStack_38.setLength(speed);
        pDodoryu->mVelocity.set(auStack_38);
    }

    // calcEscapeDir
};  // namespace DodoryuUtil

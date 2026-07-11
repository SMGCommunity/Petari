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

    // turnToward

    void accelerate(Dodoryu* pDodoryu, const TVec3f& rVec, f32 param3, f32 param4, f32 param5) {
        TVec3f auStack_40;
        TVec3f auStack_34 = TVec3f(rVec);
        auStack_40.scaleAdd(auStack_34, pDodoryu->mVelocity, -auStack_34.dot(pDodoryu->mVelocity));
        pDodoryu->mVelocity -= auStack_40;
        auStack_40 *= param4;
        pDodoryu->mVelocity += auStack_40;
        pDodoryu->mVelocity += auStack_34 * param3;

        if (pDodoryu->mVelocity.length() > param5) {
            pDodoryu->mVelocity.setLength(param5);
        }

        TVec3f yDir;
        pDodoryu->mBaseMtx.getYDir(yDir);
        pDodoryu->mVelocity.scaleAdd(yDir, pDodoryu->mVelocity, -yDir.dot(pDodoryu->mVelocity));
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
        TVec3f auStack_38;
        TVec3f auStack_2c;
        pDodoryu->mBaseMtx.getYDir(auStack_2c);

        f32 speed = pDodoryu->mVelocity.length();

        auStack_38.scaleAdd(pDodoryu->_134, auStack_38, -pDodoryu->_134.dot(auStack_38));

        if (MR::isNearZero(auStack_38)) {
            auStack_38.cross(auStack_2c, pDodoryu->_134);
        }

        auStack_38.setLength(speed);
        pDodoryu->mVelocity.set(auStack_38);
    }

    // calcEscapeDir
};  // namespace DodoryuUtil

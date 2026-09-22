#include "Game/Boss/DodoryuUtil.hpp"
#include "Game/Boss/Dodoryu.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"

void DodoryuUtil_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.0000038146973f;
    (void)0.5f;
    (void)3.1415927f;
    (void)2.0f;
}

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

    void turnToward(Dodoryu* pDodoryu, const TVec3f& rDirection, f32 maxAngle) {
        TVec3f up;
        TVec3f front;
        TVec3f position;
        pDodoryu->mBaseMtx.getYDir(up);
        pDodoryu->mBaseMtx.getZDir(front);
        pDodoryu->mBaseMtx.getTrans(position);

        f32 maxRadian = maxAngle * MR::pi() / 180.0f;

        TPos3f mtx;
        mtx.makeRotate(front, rDirection, maxRadian);
        mtx.concat(pDodoryu->mBaseMtx);
        mtx.setTrans(position);
        pDodoryu->setMtx(mtx);
    }

    void accelerate(Dodoryu* pDodoryu, const TVec3f& rDirection, f32 acceleration, f32 lateralDamping, f32 maxSpeed) {
        TVec3f direction(rDirection);
        TVec3f& rVelocity = pDodoryu->mVelocity;
        TVec3f lateralVelocity;
        lateralVelocity.killElement(rVelocity, direction);
        rVelocity -= lateralVelocity;
        lateralVelocity *= lateralDamping;
        rVelocity += lateralVelocity;
        rVelocity += direction * acceleration;

        if (rVelocity.length() > maxSpeed) {
            rVelocity.setLength(maxSpeed);
        }

        TVec3f yDir;
        pDodoryu->mBaseMtx.getYDir(yDir);
        rVelocity.orthogonalize(yDir);
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
        TVec3f up;
        pDodoryu->mBaseMtx.getYDir(up);

        f32 speed = pDodoryu->mVelocity.length();
        TVec3f velocity(pDodoryu->mVelocity);
        velocity.orthogonalize(pDodoryu->_134);

        if (MR::isNearZero(velocity)) {
            velocity.cross(up, pDodoryu->_134);
        }

        velocity.setLength(speed);
        pDodoryu->mVelocity.set(velocity);
    }

    void calcEscapeDir(Dodoryu* pDodoryu, TVec3f* pDirection, f32 maxAngle) {
        TVec3f up;
        const TVec3f* pPlayerPos = MR::getPlayerPos();
        TVec3f away(pDodoryu->mPosition);
        away.sub(*pPlayerPos);
        pDirection->set(away);
        if (!calcVerticalizedDir(pDodoryu, pDirection, *pDirection)) {
            pDodoryu->mBaseMtx.getZDir(*pDirection);
        }

        pDodoryu->mBaseMtx.getYDir(up);
        const f32 randomAngle = 2.0f * maxAngle * (MR::getRandom() - 0.5f);
        f32 angle = randomAngle * PI / 180.0f;
        TPos3f rotation;
        rotation.makeRotate(up, angle);
        rotation.mult33(*pDirection);
    }
}  // namespace DodoryuUtil

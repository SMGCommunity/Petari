#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Animation/Xanimecore.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JGeometry/TUtil.hpp"
#include "JSystem/JMath/JMATrigonometric.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include <cmath>

namespace {

    // NOTE: This inline is used in various places in the codebase.
    // TODO: figure out where exactly to put this
    inline void DO_ROTATE(const TPos3f& rMtx, TVec3f* pVec, f32 f1) {
        if (3.0f < f1) {
            if (rMtx[2][0] - 1.0f >= -(f32)JGeometry::TUtil< f32 >::epsilon()) {
                pVec->set(JMAATan2(-rMtx[0][1], rMtx[1][1]), -HALF_PI, 0.0f);
            } else if (rMtx[2][0] + 1.0f <= (f32)JGeometry::TUtil< f32 >::epsilon()) {
                pVec->set(JMAATan2(rMtx[0][1], rMtx[1][1]), HALF_PI, 0.0f);
            } else {
                pVec->x = JMAATan2(rMtx[2][1], rMtx[2][2]);
                pVec->z = JMAATan2(rMtx[1][0], rMtx[0][0]);
                pVec->y = JGeometry::TUtil< f32 >::asin(-rMtx[2][0]);
            }
        } else {
            pVec->x = std::atan2(rMtx[2][1], rMtx[2][2]);
            pVec->z = std::atan2(rMtx[1][0], rMtx[0][0]);
            pVec->y = asin(-rMtx[2][0]);
        }
    }

    void calcRotate(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TPos3f stack_44;
        TVec3f stack_38;
        TVec3f stack_2c;
        TVec3f stack_20;
        TVec3f stack_14;
        if (MR::isBindedGround(pActor)) {
            stack_38.set(*MR::getGroundNormal(pActor));
        } else {
            TVec3f stack_8 = -pActor->mGravity;
            stack_38.set(stack_8);
        }
        MR::calcUpVec(&stack_2c, pActor);
        MR::vecBlend(stack_2c, stack_38, &stack_20, 0.1f);
        MR::normalizeOrZero(&stack_20);
        if (MR::isNearZero(stack_20)) {
            stack_20.set(stack_2c);
        }
        MR::makeMtxUpFront(&stack_44, stack_20, a2);

        DO_ROTATE(stack_44, &stack_14, a3);

        pActor->mRotation.set< f32 >(stack_14.x * _180_PI, stack_14.y * _180_PI, stack_14.z * _180_PI);
    }

};  // namespace

namespace MR {
    f32 calcDistance(const HitSensor* pSensor1, const HitSensor* pSensor2, TVec3f* a3) {
        TVec3f sensor2_pos = pSensor2->mPosition - pSensor1->mPosition;

        f32 mag = sensor2_pos.length();
        if (a3 != nullptr) {
            if (mag > 0.0f) {
                a3->scale(1.0f / mag, sensor2_pos);
            } else {
                a3->zero();
            }
        }

        return mag;
    }

    f32 calcDistance(const LiveActor* pActor, const TVec3f& rVec) {
        return pActor->mPosition.distance(rVec);
    }

    f32 calcDistanceHorizontal(const LiveActor* pActor, const TVec3f& a2, const TVec3f& a3) {
        TVec3f stack_8 = a2 - pActor->mPosition;
        MR::vecKillElement(stack_8, a3, &stack_8);
        return stack_8.length();
    }

    f32 calcDistanceVertical(const LiveActor* pActor, const TVec3f& a2, const TVec3f& a3) {
        TVec3f stack_8 = a2 - pActor->mPosition;
        stack_8.scale(a3.dot(stack_8), a3);
        return stack_8.length();
    }

    f32 calcDistanceVertical(const LiveActor* pActor, const TVec3f& a2) {
        const TVec3f& grav = pActor->mGravity;
        TVec3f stack_8 = a2 - pActor->mPosition;
        stack_8.scale(grav.dot(stack_8), grav);
        return stack_8.length();
    }

    f32 calcDistanceToPlayer(const LiveActor* pActor) {
        if (!MR::isExistSceneObj(SceneObj_MarioHolder)) {
            return FLOAT_MAX;
        }

        return pActor->mPosition.distance(*MR::getPlayerPos());
    }

    f32 calcDistanceToPlayerH(const LiveActor* pActor) {
        if (MR::isExistSceneObj(SceneObj_MarioHolder) == false) {
            return FLOAT_MAX;
        }
        TVec3f stack_8 = *MR::getPlayerPos() - pActor->mPosition;
        MR::vecKillElement(stack_8, pActor->mGravity, &stack_8);
        return stack_8.length();
    }

    bool isNear(const HitSensor* pSensor_1, const HitSensor* pSensor_2, f32 dist) {
        f32 sqr = pSensor_1->mPosition.squared(pSensor_2->mPosition);
        return sqr < (dist * dist);
    }

    bool isNear(const LiveActor* pActor_1, const LiveActor* pActor_2, f32 dist) {
        f32 sqr = pActor_1->mPosition.squared(pActor_2->mPosition);
        return sqr < (dist * dist);
    }

    bool isNear(const LiveActor* pActor, const TVec3f& rVec, f32 dist) {
        f32 sqr = pActor->mPosition.squared(rVec);
        return sqr < (dist * dist);
    }

    bool isNearPlayer(const LiveActor* pActor, f32 dist) {
        return (MR::isPlayerHidden() ? false : MR::isNearPlayerAnyTime(pActor, dist));
    }

    bool isNearPlayerAnyTime(const LiveActor* pActor, f32 dist) {
        f32 sqr = pActor->mPosition.squared(*MR::getPlayerPos());
        return sqr < (dist * dist);
    }

    bool isNearPlayer(const TVec3f& rPos, f32 dist) {
        if (MR::isPlayerHidden()) {
            return false;
        }

        f32 sqr = MR::getPlayerPos()->squared(rPos);
        return sqr < (dist * dist);
    }

    bool isNearPlayerPose(const LiveActor* pActor, f32 a2, f32 a3) {
        if (isPlayerHidden()) {
            return false;
        }
        TVec3f stack_20;
        TVec3f stack_14;
        ((TRot3f*)pActor->getBaseMtx())->getYDir(stack_20);
        TVec3f stack_8 = pActor->mPosition - *MR::getPlayerPos();
        f32 f0 = __fabsf(vecKillElement(stack_8, stack_20, &stack_14));
        if (a3 < f0) {
            return false;
        }
        return (stack_14.squared() < a2 * a2);
    }

    bool isNearPlayerHorizontal(const LiveActor* pActor, f32 dist) {
        TVec3f stack_14;
        if (isPlayerHidden()) {
            return false;
        }
        TVec3f stack_8 = pActor->mPosition - *MR::getPlayerPos();
        f32 f1 = vecKillElement(stack_8, pActor->mGravity, &stack_14);
        return (stack_14.squared() < dist * dist);
    }

    void calcFrontVec(TVec3f* pFrontVec, const LiveActor* pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pFrontVec->set(mtx[0][2], mtx[1][2], mtx[2][2]);
    }

    void calcUpVec(TVec3f* pUpVec, const LiveActor* pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pUpVec->set(mtx[0][1], mtx[1][1], mtx[2][1]);
    }

    void calcSideVec(TVec3f* pSideVec, const LiveActor* pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pSideVec->set(mtx[0][0], mtx[1][0], mtx[2][0]);
    }

    void calcPositionUpOffset(TVec3f* a1, const LiveActor* pActor, f32 a3) {
        TVec3f stack_8;
        TVec3f* stack_c = a1;
        MR::calcUpVec(&stack_8, pActor);

        JMAVECScaleAdd((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)stack_c, a3);
    }

    void calcVecToPlayerH(TVec3f* pToPlayerHVec, const LiveActor* pActor, const TVec3f* a3) {
        calcVecToTargetPosH(pToPlayerHVec, pActor, *MR::getPlayerPos(), a3);
    }

    void calcVecFromPlayerH(TVec3f* pFromPlayerHVec, const LiveActor* pActor) {
        calcVecToTargetPosH(pFromPlayerHVec, pActor, *MR::getPlayerPos(), nullptr);
        pFromPlayerHVec->scale(-1.0f);
    }

    void calcVecToTargetPosH(TVec3f* pToTargetHVec, const LiveActor* pActor, const TVec3f& a3, const TVec3f* a4) {
        pToTargetHVec->set< f32 >(a3);
        pToTargetHVec->sub(pActor->mPosition);

        if (a4 == nullptr) {
            MR::vecKillElement(*pToTargetHVec, pActor->mGravity, pToTargetHVec);
        } else {
            MR::vecKillElement(*pToTargetHVec, *a4, pToTargetHVec);
        }

        MR::normalizeOrZero(pToTargetHVec);
    }

    void calcVecFromTargetPosH(TVec3f* pFromTargetHVec, const LiveActor* pActor, const TVec3f& a3, const TVec3f* a4) {
        calcVecToTargetPosH(pFromTargetHVec, pActor, a3, a4);
        pFromTargetHVec->scale(-1.0f);
    }

    bool isFaceToTargetDegree(const LiveActor* pActor, const TVec3f& a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a2 - pActor->mPosition;
        return MR::isNearAngleDegree(stack_8, a3, a4);
    }

    bool isFaceToPlayerDegree(const LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_8 = *MR::getPlayerPos() - pActor->mPosition;
        return MR::isNearAngleDegree(stack_8, a2, a3);
    }

    bool isFaceToTargetDegree(const LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8 = a2 - pActor->mPosition;

        return MR::isNearAngleDegree(stack_8, stack_14, a3);
    }

    bool isFaceToPlayerDegree(const LiveActor* pActor, f32 a2) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8 = *MR::getPlayerPos() - pActor->mPosition;

        return MR::isNearAngleDegree(stack_8, stack_14, a2);
    }

    bool isFaceToPlayerDegreeHV(const LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4) {
        TVec3f stack_8 = *MR::getPlayerPos() - pActor->mPosition;

        return MR::isNearAngleDegreeHV(stack_8, a2, pActor->mGravity, a3, a4);
    }

    bool isFaceToTargetHorizontalDegree(const LiveActor* pActor, const TVec3f& a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_20;
        TVec3f stack_14;

        TVec3f stack_8 = a2 - pActor->mPosition;

        const TVec3f* grav = &pActor->mGravity;
        JMAVECScaleAdd((Vec*)grav, (Vec*)&stack_8, (Vec*)&stack_20, -pActor->mGravity.dot(stack_8));
        JMAVECScaleAdd((Vec*)grav, (Vec*)&a3, (Vec*)&stack_14, -pActor->mGravity.dot(a3));

        return MR::isNearAngleDegree(stack_20, stack_14, a4);
    }

    bool isFaceToPlayerHorizontalDegree(const LiveActor* pActor, const TVec3f& a2, f32 a3) {
        return MR::isFaceToTargetHorizontalDegree(pActor, *MR::getPlayerPos(), a2, a3);
    }

    bool isClockwiseToDir(const LiveActor* pActor, const TVec3f& v1, const TVec3f& v2) {
        TVec3f stack_14;
        TVec3f stack_8 = pActor->mGravity.cross(v2);
        normalizeOrZero(stack_8, &stack_14);
        return (stack_14.dot(v1) >= 0.0f);
    }

    bool isClockwiseToPlayer(const LiveActor* pActor, const TVec3f& v1) {
        TVec3f stack_20 = *MR::getPlayerPos() - pActor->mPosition;
        TVec3f stack_14 = pActor->mGravity.cross(stack_20);
        TVec3f stack_8;
        normalizeOrZero(stack_14, &stack_8);
        return (stack_8.dot(v1) >= 0.0f);
    }

    bool isPlayerLeftSide(const LiveActor* pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        TVec3f sideVec = TVec3f(mtx[0][0], mtx[1][0], mtx[2][0]);
        TVec3f stack_8 = *MR::getPlayerPos() - pActor->mPosition;
        return (stack_8.dot(sideVec) >= 0.0f);
    }

    bool isLeftSideFromPlayer(const LiveActor* pActor) {
        TVec3f stack_14;
        getPlayerSideVec(&stack_14);
        TVec3f stack_8 = pActor->mPosition - *MR::getPlayerPos();
        return (stack_14.dot(stack_8) >= 0.0f);
    }

    bool isInSightConePlayer(const LiveActor* pActor, const TVec3f& a2, f32 dist, f32 angle) {
        return isNearPlayerAnyTime(pActor, dist) && isFaceToPlayerDegree(pActor, a2, angle);
    }

    bool isInSightFanPlayer(const LiveActor* pActor, const TVec3f& a2, f32 dist, f32 a3, f32 a4) {
        return isNearPlayerAnyTime(pActor, dist) && isFaceToPlayerDegreeHV(pActor, a2, a3, a4);
    }

    bool isBindedWallFront(const LiveActor* pActor, const TVec3f& v1, f32 a3) {
        if (isBindedWall(pActor)) {
            TVec3f wallNormal(*getWallNormal(pActor));  // stack_8
            if (v1.dot(wallNormal) < -a3) {
                return true;
            }
        }
        return false;
    }

    bool isOnPlayer(const LiveActor* pActor) {
        return isActorOnPlayer(pActor);
    }

    bool isPlayerExistSide(const LiveActor* pActor, f32 a2, f32 a3) {
        TVec3f stack_2c = *getPlayerCenterPos();
        TVec3f upVec;
        calcUpVec(&upVec, pActor);
        TVec3f stack_14 = pActor->mPosition + upVec * a2;
        stack_2c.sub(stack_14);
        normalizeOrZero(&stack_2c);
        f32 f1 = upVec.dot(stack_2c);
        if (f1 < 0.0f) {
            f1 = -f1;
        }
        if (f1 < a3) {
            return true;
        }
        return false;
    }

    bool isPlayerExistUp(const LiveActor* pActor, f32 a2, f32 a3) {
        TVec3f stack_2c = *getPlayerCenterPos();
        TVec3f upVec;
        calcUpVec(&upVec, pActor);
        TVec3f stack_14 = pActor->mPosition + upVec * a2;
        stack_2c.sub(stack_14);
        normalizeOrZero(&stack_2c);
        return (a3 < upVec.dot(stack_2c));
    }

    bool isExistPlayerDown(const LiveActor* pActor, f32 a2, f32 a3) {
        TVec3f stack_38 = *getPlayerCenterPos();
        TVec3f upVec;
        calcUpVec(&upVec, pActor);
        upVec = -upVec;
        TVec3f stack_14 = pActor->mPosition + upVec * a2;
        stack_38.sub(stack_14);
        normalizeOrZero(&stack_38);
        return (a3 < upVec.dot(stack_38));
    }

    bool isInWater(const LiveActor* pActor, const TVec3f& rOffset) {
        return MR::isInWater(pActor->mPosition + rOffset);
    }

    bool isInDeath(const LiveActor* pActor, const TVec3f& rOffset) {
        return MR::isInDeath(pActor->mPosition + rOffset);
    }

    void makeMtxTR(MtxPtr mtx, const LiveActor* pActor) {
        MR::makeMtxTR(mtx, pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z, pActor->mRotation.x, pActor->mRotation.y,
                      pActor->mRotation.z);
    }

    void makeMtxTRS(MtxPtr mtx, const LiveActor* pActor) {
        MR::makeMtxTRS(mtx, pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z, pActor->mRotation.x, pActor->mRotation.y,
                       pActor->mRotation.z, pActor->mScale.x, pActor->mScale.y, pActor->mScale.z);
    }

    void makeMtxTransRotateY(MtxPtr mtx, const LiveActor* pActor) {
        MR::makeMtxTransRotateY(mtx, pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z, pActor->mRotation.y);
    }

    void calcMtxFromGravityAndZAxis(TPos3f* pResult, const LiveActor* pActor, const TVec3f& vGravity, const TVec3f& zAxis) {
        TPos3f pos;  // stack_44
        pos.setInline(pActor->getBaseMtx());
        TVec3f yDir;   // stack_38
        TVec3f zDir;   // stack_2c
        TVec3f xDir;   // stack_20
        TVec3f xDir2;  // stack_14
        if (!isNearZero(vGravity)) {
            TVec3f stack_8 = -vGravity;
            yDir = stack_8;
        } else {
            pos.getYDir(yDir);
        }
        if (!isNearZero(zAxis)) {
            zDir = zAxis;
        } else {
            pos.getZDir(zDir);
        }
        xDir.cross(zDir, yDir);
        if (isNearZero(xDir)) {
            pos.getXDir(xDir2);
            zDir.cross(xDir2, yDir);
        }
        makeMtxUpFrontPos(pResult, yDir, zDir, pActor->mPosition);
    }

    void calcAndSetBaseMtxFromGravityAndZAxis(LiveActor* pActor, const TVec3f& zAxis) {
        TPos3f stack_8;
        stack_8.identity();
        calcMtxFromGravityAndZAxis(&stack_8, pActor, pActor->mGravity, zAxis);
        setBaseTRMtx(pActor, stack_8);
    }

    void calcActorAxis(TVec3f* outputX, TVec3f* outputY, TVec3f* outputZ, const LiveActor* pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getXDir(*outputX);
        stack_8.getYDir(*outputY);
        stack_8.getZDir(*outputZ);
    }

    void calcActorAxisX(TVec3f* pDest, const LiveActor* pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getXDir(*pDest);
    }

    void calcActorAxisY(TVec3f* pDest, const LiveActor* pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getYDir(*pDest);
    }

    void calcActorAxisZ(TVec3f* pDest, const LiveActor* pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getZDir(*pDest);
    }

    bool faceToVector(TQuat4f* pQuat, TVec3f a2, f32 a3) {
        TVec3f yDir;  // stack_8
        pQuat->getYDir(yDir);
        normalizeOrZero(&a2);
        if (vecKillElement(a2, yDir, &a2) > 0.95f) {
            return false;
        } else {
            return turnQuatZDirRad(pQuat, *pQuat, a2, a3 * PI_180);
        }
    }

    bool faceToPoint(MtxPtr mtx, TVec3f a2, f32 a3) {
        a2.sub(TVec3f(mtx[0][3], mtx[1][3], mtx[2][3]));
        return faceToVector(mtx, a2, a3);
    }

    void makeQuatFromRotate(TQuat4f* pQuat, const LiveActor* pActor) {
        makeQuatRotateDegree(pQuat, pActor->mRotation);
        return;
    }

    void makeQuatAndFrontFromRotate(TQuat4f* pQuat, TVec3f* pVec, const LiveActor* pActor) {
        makeQuatRotateDegree(pQuat, pActor->mRotation);
        pQuat->getZDir(*pVec);
        return;
    }

    void turnQuatUpToGravity(TQuat4f* pQuatDest, const TQuat4f& pQuatSrc, const LiveActor* pActor) {
        TVec3f yDir;
        pQuatSrc.getYDir(yDir);

        TQuat4f q1;
        if (yDir.dot(-pActor->mGravity) <= -0.999f) {
            TVec3f xDir;
            pQuatSrc.getXDir(xDir);
            q1.setRotate(xDir, PI);
        } else {
            q1.setRotate(yDir, -pActor->mGravity);
        }
        PSQUATMultiply(reinterpret_cast< const Quaternion* >(&q1), reinterpret_cast< const Quaternion* >(&pQuatSrc),
                       reinterpret_cast< Quaternion* >(pQuatDest));
        pQuatDest->normalize();
    }

    void blendQuatFromGroundAndFront(TQuat4f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4, f32 a5) {
        MR::blendQuatUpFront(a1, MR::isBindedGround(pActor) ? *MR::getGroundNormal(pActor) : -pActor->mGravity, a3, a4, a5);
    }

    void resetPosition(LiveActor* pActor) {
        MR::clearHitSensors(pActor);

        if (pActor->mBinder != nullptr) {
            pActor->mBinder->clear();
        }

        if (MR::isCalcGravity(pActor)) {
            MR::calcGravity(pActor);
        }

        MR::calcAnimDirect(pActor);

        if (pActor->mCollisionParts != nullptr) {
            MR::resetAllCollisionMtx(pActor);
        }

        MR::requestCalcActorShadowAppear(pActor);
    }

    void resetPosition(LiveActor* pActor, const TVec3f& rPosition) {
        pActor->mPosition.set< f32 >(rPosition);

        MR::resetPosition(pActor);
    }

    void resetPosition(LiveActor* pActor, const char* a2) {
        TVec3f pos(0.0f, 0.0f, 0.0f);
        TVec3f rot(0.0f, 0.0f, 0.0f);
        MR::findNamePos(a2, &pos, &rot);

        pActor->mPosition.set< f32 >(pos);
        pActor->mRotation.set< f32 >(rot);

        MR::resetPosition(pActor);
    }

    void calcVelocityMoveToDirectionHorizon(TVec3f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4) {
        TVec3f* pGravity = const_cast< TVec3f* >(&pActor->mGravity);
        a1->rejection(a3, *pGravity);
        normalizeOrZero(a1);
        a1->scale(a4);
    }

    void calcVelocityMoveToDirectionHorizon(TVec3f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        TVec3f* pGravity = const_cast< TVec3f* >(&pActor->mGravity);
        a1->rejection(a3, *pGravity);
        f32 stack_8;
        separateScalarAndDirection(&stack_8, a1, *a1);
        a1->scale(getInterpolateValue(normalize(stack_8, a6, a7), a4, a5));
    }

    void calcVelocityMoveToDirection(TVec3f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4) {
        calcVelocityMoveToDirectionHorizon(a1, pActor, a3, a4);
        if (isOnGround(pActor)) {
            const TVec3f* pGroundNormal = getGroundNormal(pActor);
            JMAVECScaleAdd((Vec*)pGroundNormal, (Vec*)a1, (Vec*)a1, -pGroundNormal->dot(*a1));
        }
    }

    void calcVelocityMoveToDirection(TVec3f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        calcVelocityMoveToDirectionHorizon(a1, pActor, a3, a4, a5, a6, a7);
        if (isOnGround(pActor)) {
            const TVec3f* pGroundNormal = getGroundNormal(pActor);
            JMAVECScaleAdd((Vec*)pGroundNormal, (Vec*)a1, (Vec*)a1, -pGroundNormal->dot(*a1));
        }
    }

    void calcVelocityMoveToTarget(TVec3f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        calcVelocityMoveToDirection(a1, pActor, stack_8, a4);
    }

    void addVelocityMoveToDirection(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, a2, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityMoveToTarget(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_14 = a2 - pActor->mPosition;
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityMoveToTarget(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14 = a2 - pActor->mPosition;
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3, a4, a5, a6);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityAwayFromTarget(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_14 = pActor->mPosition - a2;
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityAwayFromTarget(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14 = pActor->mPosition - a2;
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3, a4, a5, a6);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityClockwiseToDirection(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_14;
        if (normalizeOrZero(a2, &stack_14)) {
            return;
        }
        stack_14.cross(pActor->mGravity, stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityClockwiseToTarget(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        TVec3f stack_8 = a2 - pActor->mPosition;
        addVelocityClockwiseToDirection(pActor, stack_8, a3);
    }

    void addVelocityClockwiseToPlayer(LiveActor* pActor, f32 a2) {
        TVec3f stack_8 = *getPlayerPos() - pActor->mPosition;
        addVelocityClockwiseToDirection(pActor, stack_8, a2);
    }

    void addVelocityJump(LiveActor* pActor, f32 a2) {
        (pActor->mVelocity).add(pActor->mGravity * -a2);
    }

    bool addVelocityLimit(LiveActor* pActor, const TVec3f& a2) {
        TVec3f stack_18;
        f32 stack_8;
        separateScalarAndDirection(&stack_8, &stack_18, a2);
        if (isNearZero(stack_18)) {
            return false;
        }
        f32 f1 = stack_18.dot(pActor->mVelocity);
        if (stack_8 <= f1) {
            return false;
        }
        pActor->mVelocity.add(stack_18 * (stack_8 - f1));
        return true;
    }

    void setVelocityJump(LiveActor* pActor, f32 a2) {
        (pActor->mVelocity).set(pActor->mGravity * -a2);
    }

    void addVelocityToGravity(LiveActor* pActor, f32 a2) {
        (pActor->mVelocity).add(pActor->mGravity * a2);
    }

    void addVelocityToGravityOrGround(LiveActor* pActor, f32 a2) {
        if (isBindedGround(pActor)) {
            JMAVECScaleAdd((Vec*)getGroundNormal(pActor), (Vec*)&(pActor->mVelocity), (Vec*)&(pActor->mVelocity), -a2);
        } else {
            addVelocityToGravity(pActor, a2);
        }
    }

    bool addVelocityToCollisionNormal(LiveActor* pActor, f32 a2) {
        if (isBinded(pActor) == false) {
            return false;
        } else {
            TVec3f stack_14(*getBindedFixReactionVector(pActor));
            if (isNearZero(stack_14)) {
                return false;
            }
            normalize(&stack_14);
            (pActor->mVelocity).add(stack_14 * a2);
            return true;
        }
    }

    void addVelocityKeepHeight(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4) {
        addVelocityKeepHeight(pActor, a2, 0.0f, a3, a4);
    }

    void addVelocityKeepHeight(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4, f32 a5) {
        TVec3f stack_8 = a2 - pActor->mPosition;
        addVelocityToGravity(pActor, a4 * normalizeAbs((pActor->mGravity).dot(stack_8) - a3, -a5, a5));
    }

    bool addVelocityKeepHeightUseShadow(LiveActor* pActor, f32 a2, f32 a3, f32 a4, const char* pName) {
        if (pName != nullptr) {
            if (isShadowProjected(pActor, pName) == false) {
                return false;
            }
        } else {
            if (isShadowProjectedAny(pActor) == false) {
                return false;
            }
        }
        f32 shadowLength;
        if (pName != nullptr) {
            shadowLength = getShadowProjectionLength(pActor, pName);
        } else {
            shadowLength = getShadowNearProjectionLength(pActor);
        }
        shadowLength -= a2;
        addVelocityToGravity(pActor, a3 * normalizeAbs(shadowLength, -a4, a4));
        return true;
    }

    bool addVelocityKeepHeightUseShadow(LiveActor* pActor, f32 a2, f32 a3, f32 a4, f32 a5, const char* pName) {
        if (pName != nullptr) {
            if (isShadowProjected(pActor, pName) == false) {
                return false;
            }
        } else {
            if (isShadowProjectedAny(pActor) == false) {
                return false;
            }
        }
        f32 f1;
        if (pName != nullptr) {
            f1 = getShadowProjectionLength(pActor, pName);
        } else {
            f1 = getShadowNearProjectionLength(pActor);
        }
        f1 -= a2;
        f1 = normalizeAbs(f1, -a5, a5);
        if (f1 >= 0.0f) {
            addVelocityToGravity(pActor, a3 * f1);
        } else {
            addVelocityToGravity(pActor, a4 * f1);
        }
        return true;
    }

    void addVelocitySeparateHV(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4) {
        TVec3f stack_2c;
        stack_2c.rejection(a2, pActor->mGravity);
        normalizeOrZero(&stack_2c);
        TVec3f stack_20 = stack_2c * a3 - pActor->mGravity * a4;
        pActor->mVelocity.add(stack_20);
    }

    void setVelocitySeparateHV(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4) {
        TVec3f stack_2c;
        stack_2c.rejection(a2, pActor->mGravity);
        normalizeOrZero(&stack_2c);
        TVec3f stack_20 = stack_2c * a3 - pActor->mGravity * a4;
        pActor->mVelocity.set(stack_20);
    }

    void attenuateVelocity(LiveActor* pActor, f32 scalar) {
        (pActor->mVelocity).scale(scalar);
    }

    void attenuateVelocityExceptDirection(LiveActor* pActor, const TVec3f& a2, f32 a3) {
        // FIXME: load order for pVelocity->scale
        // https://decomp.me/scratch/2CjA5

        TVec3f* pVelocity = &pActor->mVelocity;
        TVec3f stack_8;
        stack_8.rejection(pActor->mVelocity, a2);
        stack_8.scale(a3);
        pVelocity->scale(a2.dot(pActor->mVelocity), a2);
        pVelocity->add(stack_8);
    }

    void restrictVelocity(LiveActor* pActor, f32 speed) {
        if ((pActor->mVelocity).squared() > speed * speed) {
            (pActor->mVelocity).setLength(speed);
        }
    }

    void killVelocityOnGroundH(LiveActor* pActor) {
        if (isOnGround(pActor)) {
            const TVec3f* pNormal = getGroundNormal(pActor);
            pActor->mVelocity.scale(pNormal->dot(pActor->mVelocity), *pNormal);
        }
    }

    void killVelocityOnGroundCosH(LiveActor* pActor, f32 a2) {
        if (isOnGround(pActor) && isFloorPolygonCos(*getGroundNormal(pActor), pActor->mGravity, a2)) {
            const TVec3f* pNormal = getGroundNormal(pActor);
            pActor->mVelocity.scale(pNormal->dot(pActor->mVelocity), *pNormal);
        }
    }

    // Minor mismatch: Wrong register used for pVelocity
    void killVelocityToTarget(LiveActor* pActor, const TVec3f& a2) {
        TVec3f stack_8;
        stack_8.sub(a2, pActor->mPosition);
        normalize(&stack_8);
        if (pActor->mVelocity.dot(stack_8) > 0.0f) {
            TVec3f* pVelocity = &pActor->mVelocity;
            pVelocity->rejection(*pVelocity, stack_8);
        }
    }

    void forceBindOnGround(LiveActor* pActor, f32 a2, f32 a3) {
        if (isShadowProjected(pActor, nullptr)) {
            TVec3f stack_20;
            getShadowProjectionPos(pActor, nullptr, &stack_20);
            stack_20.sub(pActor->mPosition);
            f32 f1 = stack_20.dot(pActor->mGravity);
            if (0.0f != a2) {
                f1 -= a2;
            }
            stack_20.set(pActor->mGravity * f1);
            addVelocityLimit(pActor, stack_20 * a3);
        }
    }

    bool reboundVelocityFromEachCollision(LiveActor* pActor, f32 a2, f32 a3, f32 a4, f32 a5) {
        if (!isBinded(pActor)) {
            return false;
        }
        TVec3f bindedReactnVec = *getBindedFixReactionVector(pActor);
        if (isNearZero(bindedReactnVec)) {
            return false;
        }
        normalize(&bindedReactnVec);
        f32 f31 = bindedReactnVec.dot(pActor->mGravity);
        f32 f30 = 0.0f;
        if (isFloorPolygon(f31)) {
            f30 = a2;
        } else if (isWallPolygon(f31)) {
            f30 = a3;
        } else if (isCeilingPolygon(f31)) {
            f30 = a4;
        }
        f31 = bindedReactnVec.dot(pActor->mVelocity);
        f32 f0 = -a5;
        if (f31 < f0) {
            pActor->mVelocity.sub(bindedReactnVec * f31 * (1.0f + f30));
            return true;
        }
        if (f31 < 0.0f) {
            pActor->mVelocity.sub(bindedReactnVec * f31);
            return false;
        }
        return false;
    }

    bool reboundVelocityFromCollision(LiveActor* pActor, f32 a2, f32 a3, f32 a4) {
        if (!isBinded(pActor)) {
            return false;
        }
        TVec3f bindedReactnVec = *getBindedFixReactionVector(pActor);
        if (isNearZero(bindedReactnVec)) {
            return false;
        }
        normalize(&bindedReactnVec);
        f32 f31 = bindedReactnVec.dot(pActor->mVelocity);
        if (f31 < -a3) {
            pActor->mVelocity.sub(bindedReactnVec * f31);
            pActor->mVelocity.scale(a4);
            pActor->mVelocity.sub(bindedReactnVec * f31 * a2);
            return true;
        }
        if (f31 < 0.0f) {
            pActor->mVelocity.sub(bindedReactnVec * f31);
            return false;
        }
        return false;
    }

    void zeroVelocity(LiveActor* pActor) {
        (pActor->mVelocity).zero();
    }

    void setVelocity(LiveActor* pActor, const TVec3f& speed) {
        (pActor->mVelocity).set(speed);
    }

    void addVelocity(LiveActor* pActor, const TVec3f& speed) {
        (pActor->mVelocity).add(speed);
    }

    void scaleVelocity(LiveActor* pActor, f32 scaleFactor) {
        (pActor->mVelocity).scale(scaleFactor);
    }

    f32 calcVelocityLength(const LiveActor* pActor) {
        return pActor->mVelocity.length();
    }

    f32 calcGravitySpeed(const LiveActor* pActor) {
        return (pActor->mVelocity).dot(pActor->mGravity);
    }

    // Two instruction swaps
    void applyVelocityDampAndGravity(LiveActor* pActor, f32 a2, f32 groundedScalar, f32 airborneScalar, f32 fallingScalar, f32 a6) {
        TVec3f stack_38;
        TVec3f stack_2c;
        if (!isBindedGround(pActor)) {
            pActor->mVelocity.add(pActor->mGravity * a2);
        }
        TVec3f* pVelocity = &pActor->mVelocity;
        TVec3f* pGravThenVel = &pActor->mGravity;
        stack_38.rejection(*pVelocity, *pGravThenVel);
        stack_2c.scale(pGravThenVel->dot(*pVelocity), *pGravThenVel);
        if (isOnGround(pActor)) {
            stack_38.scale(groundedScalar);
        } else {
            stack_38.scale(airborneScalar);
        }
        if (stack_2c.dot(pActor->mGravity) < 0.0f) {
            stack_2c.scale(fallingScalar);
        }
        pActor->mVelocity.set(stack_38 + stack_2c);
        if (isOnGround(pActor)) {
            TVec3f stack_20(*getGroundNormal(pActor));
            TVec3f* pVelocity2 = &pActor->mVelocity;
            stack_38.rejection(pActor->mVelocity, stack_20);
            if (stack_38.squared() < a6 * a6) {
                pVelocity2->scale(stack_20.dot(pActor->mVelocity), stack_20);
            }
        }
    }

    void setVelocityJumpAwayFromPlayer(LiveActor* pActor, f32 a2, f32 a3) {
        TVec3f* pVelocity = &pActor->mVelocity;
        calcVecToTargetPosH(pVelocity, pActor, *getPlayerPos(), nullptr);
        pVelocity->scale(-1.0f);
        pVelocity->scale(a2);
        addVelocityJump(pActor, a3);
    }

    // Minor mismatch: Two instructions swapped in rejection
    bool sendMsgPushAndKillVelocityToTarget(LiveActor* pActor, HitSensor* pSensor1, HitSensor* pSensor2) {
        if (sendMsgPush(pSensor1, pSensor2)) {
            TVec3f stack_8 = pSensor2->mPosition - pSensor1->mPosition;
            normalizeOrZero(&stack_8);
            if (pActor->mVelocity.dot(stack_8) < 0.0f) {
                TVec3f* pVelocity = &pActor->mVelocity;
                pVelocity->rejection(*pVelocity, stack_8);
            }
            return true;
        }
        return false;
    }

    void addVelocityFromPush(LiveActor* pActor, f32 a2, HitSensor* pSensor1, HitSensor* pSensor2) {
        TVec3f stack_14 = pSensor2->mPosition - pSensor1->mPosition;
        if (a2 < (pActor->mVelocity).dot(-pActor->mGravity)) {
            vecKillElement(stack_14, pActor->mGravity, &stack_14);
        }
        stack_14.setLength(a2);
        addVelocityLimit(pActor, stack_14);
    }

    void addVelocityFromPushHorizon(LiveActor* pActor, f32 speed, HitSensor* pSensor1, HitSensor* pSensor2) {
        TVec3f stack_8 = pSensor2->mPosition - pSensor1->mPosition;
        vecKillElement(stack_8, pActor->mGravity, &stack_8);
        stack_8.setLength(speed);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocitySeparateHV(LiveActor* pActor, HitSensor* pSensor1, HitSensor* pSensor2, f32 a4, f32 a5) {
        TVec3f stack_8 = pSensor2->mPosition - pSensor1->mPosition;
        addVelocitySeparateHV(pActor, stack_8, a4, a5);
    }

    void setVelocitySeparateHV(LiveActor* pActor, HitSensor* pSensor1, HitSensor* pSensor2, f32 a4, f32 a5) {
        TVec3f stack_8 = pSensor2->mPosition - pSensor1->mPosition;
        setVelocitySeparateHV(pActor, stack_8, a4, a5);
    }

    void setVelocityBlowAttack(LiveActor* pActor, HitSensor* pSensor1, HitSensor* pSensor2, f32 a4, f32 a5, s32 a6) {
        setVelocitySeparateHV(pActor, pSensor1, pSensor2, a4, a5);
        if (isNearPlayerAnyTime(pActor, 1000.0f)) {
            stopSceneForDefaultHit(a6);
        }
    }

    // Minor mismatch: f1 used instead of f31 in the float compare
    bool reflectVelocityH(LiveActor* pActor, HitSensor* pSensor1, HitSensor* pSensor2, f32 a4) {
        TVec3f stack_14;
        calcSensorHorizonNormalize(&stack_14, (pActor->mGravity), pSensor2, pSensor1);
        if (((1.0f + a4) * pActor->mVelocity.dot(stack_14)) > 0.0f) {
            return false;
        }
        pActor->mVelocity.sub(stack_14 * a4);
        return true;
    }

    bool calcVelocityAreaMoveOnGround(TVec3f* a1, const LiveActor* pActor) {
        a1->zero();
        if ((isOnGround(pActor) == false) || (isBindedGroundAreaMove(pActor) == false)) {
            return false;
        }
        calcAreaMoveVelocity(a1, pActor->mPosition);
        return true;
    }

    bool calcVelocityAreaOrRailMoveOnGround(TVec3f* a1, const LiveActor* pActor) {
        return calcVelocityAreaMoveOnGround(a1, pActor) || calcVelocityRailMoveOnGround(a1, pActor);
    }

    void rotateDirectionGravityDegree(const LiveActor* pActor, TVec3f* a2, f32 angle) {
        rotateVecRadian(a2, (pActor->mGravity), PI_180 * angle);
    }

    void turnDirection(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        turnVecToVecCosOnPlane(a2, a3, (pActor->mGravity), a4);
    }

    void turnDirectionDegree(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        turnVecToVecCosOnPlane(a2, a3, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionToTarget(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, a4);
    }

    void turnDirectionToTargetDegree(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        turnVecToVecCosOnPlane(a2, stack_8, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionToTargetDegreeHorizon(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        vecKillElement(stack_8, pActor->mGravity, &stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, cosDegree(a4));
    }

    void turnDirectionToTargetUseGroundNormalDegree(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        a4 = cosDegree(a4);
        turnVecToVecCosOnPlane(a2, stack_8, isBindedGround(pActor) ? *getGroundNormal(pActor) : (pActor->mGravity), a4);
    }

    void turnDirectionToPlayerDegree(const LiveActor* pActor, TVec3f* a2, f32 a3) {
        turnDirectionToTargetDegree(pActor, a2, *getPlayerPos(), a3);
    }

    void turnDirectionToPlayerDegreeHorizon(const LiveActor* pActor, TVec3f* a2, f32 a3) {
        turnDirectionToTargetDegreeHorizon(pActor, a2, *getPlayerPos(), a3);
    }

    void turnDirectionFromTargetDegree(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = pActor->mPosition - a3;
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, cosDegree(a4));
    }

    void turnDirectionFromTargetDegreeHorizon(const LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4) {
        TVec3f stack_8 = pActor->mPosition - a3;
        vecKillElement(stack_8, (pActor->mGravity), &stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionFromPlayerDegree(const LiveActor* pActor, TVec3f* a2, f32 a3) {
        turnDirectionFromTargetDegree(pActor, a2, *getPlayerPos(), a3);
    }

    void turnDirectionFromPlayerDegreeHorizon(const LiveActor* pActor, TVec3f* a2, f32 a3) {
        turnDirectionFromTargetDegreeHorizon(pActor, a2, *getPlayerPos(), a3);
    }

    f32 turnDirectionAndGravityH(TQuat4f* a1, const LiveActor* pActor, const TVec3f& a3, f32 a4, f32 a5) {
        TVec3f stack_20(a3);
        vecKillElement(stack_20, pActor->mGravity, &stack_20);
        normalizeOrZero(&stack_20);
        TVec3f stack_14;
        a1->getZDir(stack_14);
        f32 f31 = 0.0f;
        f32 f30 = 1.0f;
        if (isNearZero(stack_20) == false) {
            turnVecToVecRadian(&stack_14, stack_14, stack_20, a4, pActor->mGravity);
        } else {
            a5 = 1.0f;
            f30 = 0.1f;
        }
        TVec3f stack_8 = -pActor->mGravity;
        blendQuatUpFront(a1, stack_8, stack_14, a5, f30);
        a1->normalize();
        return f31;
    }

    // Moderate mismatch: Bad instruction order when scaling the rotation
    f32 turnDirectionAndGravityH(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4) {
        TQuat4f stack_8;
        makeQuatRotateDegree(&stack_8, pActor->mRotation);
        f32 ret = turnDirectionAndGravityH(&stack_8, pActor, a2, a3, a4);
        stack_8.getEuler(pActor->mRotation);
        // NOTE: this is probably an inline in TVec?
        pActor->mRotation.set< f32 >(pActor->mRotation.x * _180_PI, pActor->mRotation.y * _180_PI, pActor->mRotation.z * _180_PI);
        return ret;
    }

    void turnDirectionToGround(const LiveActor* pActor, TVec3f* a2) {
        TVec3f stack_8;
        if (isBindedGround(pActor)) {
            stack_8.scale(-1.0f, *getGroundNormal(pActor));
        } else {
            stack_8.set(pActor->mGravity);
        }
        JMAVECScaleAdd((Vec*)&stack_8, (Vec*)a2, (Vec*)a2, -stack_8.dot(*a2));
        normalize(a2);
    }

    void moveAndTurnToDirection(LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        turnVecToVecCosOnPlane(a2, a3, pActor->mGravity, cosDegree(a7));
        turnDirectionToGround(pActor, a2);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, *a2, a4);
        pActor->mVelocity.add(stack_8);
        pActor->mVelocity.scale(a6);
        reboundVelocityFromCollision(pActor, 0.0f, 0.0f, 1.0f);
        addVelocityToGravityOrGround(pActor, a5);
    }

    void moveAndTurnToTarget(LiveActor* pActor, TVec3f* a2, const TVec3f& a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        TVec3f stack_8 = a3 - pActor->mPosition;
        moveAndTurnToDirection(pActor, a2, stack_8, a4, a5, a6, a7);
    }

    void moveAndTurnToPlayer(LiveActor* pActor, TVec3f* a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8 = *getPlayerPos() - pActor->mPosition;
        moveAndTurnToDirection(pActor, a2, stack_8, a3, a4, a5, a6);
    }

    void flyAndTurnAlongRailSearchingPlayer(LiveActor* pActor, TVec3f* a2, f32 a3, f32 a4, f32 a5, f32 a6, f32 a7, bool a8) {
        setRailDirectionCloseToNearestPos(pActor, *getPlayerCenterPos());
        TVec3f stack_20;
        calcMovingDirectionAlongRail(pActor, &stack_20, pActor->mPosition, a3, a8, nullptr);
        turnVecToVecDegree(a2, *a2, stack_20, a7, TVec3f(0, 1, 0));
        pActor->mVelocity.add(*a2 * a4);
        pActor->mVelocity.scale(a6);
        reboundVelocityFromCollision(pActor, 0.0f, 0.0f, 1.0f);
        addVelocityToGravityOrGround(pActor, a5);
    }

    void escapeFromPlayer(LiveActor* pActor, TVec3f* a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8 = pActor->mPosition - *MR::getPlayerPos();
        moveAndTurnToDirection(pActor, a2, stack_8, a3, a4, a5, a6);
    }

    void moveAndTurnToDirection(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8;
        calcFrontVec(&stack_8, pActor);
        moveAndTurnToDirection(pActor, &stack_8, a2, a3, a4, a5, a6);
        calcRotate(pActor, stack_8, a6);
    }

    void moveAndTurnToTarget(LiveActor* pActor, const TVec3f& a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14;
        calcFrontVec(&stack_14, pActor);
        normalizeOrZero(&stack_14);
        TVec3f stack_8 = a2 - pActor->mPosition;
        moveAndTurnToDirection(pActor, &stack_14, stack_8, a3, a4, a5, a6);
        calcRotate(pActor, stack_14, a6);
    }

    void moveAndTurnToPlayer(LiveActor* pActor, f32 a2, f32 a3, f32 a4, f32 a5) {
        moveAndTurnToTarget(pActor, *getPlayerPos(), a2, a3, a4, a5);
    }

    void moveAndTurnAlongRail(LiveActor* pActor, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6, bool* a7) {
        TQuat4f stack_20;
        TVec3f stack_14;
        makeQuatRotateDegree(&stack_20, pActor->mRotation);
        stack_20.getZDir(stack_14);
        TVec3f stack_8(0, 0, 0);
        calcMovingDirectionAlongRailH(pActor, &stack_8, pActor->mPosition, a2, a7);
        moveAndTurnToDirection(pActor, &stack_14, stack_8, a3, a4, a5, a6);
        ::calcRotate(pActor, stack_14, a6);
    }
};  // namespace MR

#include "Game/Util.hpp"
#include "Game/Animation/Xanimecore.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"
#include "revolution/mtx.h"
#include <cmath>

namespace MR {
    f32 calcDistance(const HitSensor *pSensor1, const HitSensor *pSensor2, TVec3f *a3) {
        TVec3f sensor2_pos(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&sensor2_pos, (Vec*)&pSensor1->mPosition, (Vec*)&sensor2_pos);

        f32 mag = PSVECMag((Vec*)&sensor2_pos);

        if (a3) {
            if (mag > 0.0f) {
                PSVECScale((Vec*)&sensor2_pos, (Vec*)a3, 1.0f / mag);
            }
            else {
                a3->zero();
            }
        }

        return mag;
    }

    f32 calcDistance(const LiveActor *pActor, const TVec3f &rVec) {
        return PSVECDistance((Vec*)&pActor->mPosition, &(const Vec &)rVec);
    }

    f32 calcDistanceHorizontal(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3) {
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);
        MR::vecKillElement(stack_8, a3, &stack_8);
        return PSVECMag((Vec*)&stack_8);
    }

    f32 calcDistanceVertical(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3) {
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);
        stack_8.scale(a3.dot(stack_8), a3);
        return PSVECMag((Vec*)&stack_8);
    }

    f32 calcDistanceVertical(const LiveActor *pActor, const TVec3f &a2) {
        const TVec3f &grav = pActor->mGravity;
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);
        stack_8.scale(grav.dot(stack_8), grav);
        return PSVECMag((Vec*)&stack_8);
    }

    f32 calcDistanceToPlayer(const LiveActor *pActor) {
        return (!MR::isExistSceneObj(SceneObj_MarioHolder) ? FLOAT_MAX : PSVECDistance((Vec*)&pActor->mPosition, (Vec*)MR::getPlayerPos()));
    }

    f32 calcDistanceToPlayerH(const LiveActor *pActor) {
        if (MR::isExistSceneObj(SceneObj_MarioHolder) == false) {
            return FLOAT_MAX;
        }
        TVec3f stack_8(*MR::getPlayerPos());
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);
        MR::vecKillElement(stack_8, pActor->mGravity, &stack_8);
        return PSVECMag((Vec*)&stack_8);
    }

    bool isNear(const HitSensor *pSensor_1, const HitSensor *pSensor_2, f32 dist) {
        f32 sqr = pSensor_1->mPosition.squared(pSensor_2->mPosition);
        return sqr < (dist * dist);
    }

    bool isNear(const LiveActor *pActor_1, const LiveActor *pActor_2, f32 dist) {
        f32 sqr = pActor_1->mPosition.squared(pActor_2->mPosition);
        return sqr < (dist * dist);
    }

    bool isNear(const LiveActor *pActor, const TVec3f &rVec, f32 dist) {
        f32 sqr = pActor->mPosition.squared(rVec);
        return sqr < (dist * dist);
    }

    bool isNearPlayer(const LiveActor *pActor, f32 dist) {
        return (MR::isPlayerHidden() ? false : MR::isNearPlayerAnyTime(pActor, dist));
    }

    bool isNearPlayerAnyTime(const LiveActor *pActor, f32 dist) {
        f32 sqr = pActor->mPosition.squared(*MR::getPlayerPos());
        return sqr < (dist * dist);
    }

    bool isNearPlayer(const TVec3f &rPos, f32 dist) {
        if (MR::isPlayerHidden()) {
            return false;
        }

        f32 sqr = MR::getPlayerPos()->squared(rPos);
        return sqr < (dist * dist);
    }

    void calcFrontVec(TVec3f *pFrontVec, const LiveActor *pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pFrontVec->set(mtx[0][2], mtx[1][2], mtx[2][2]);
    }

    void calcUpVec(TVec3f *pUpVec, const LiveActor *pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pUpVec->set(mtx[0][1], mtx[1][1], mtx[2][1]);
    }

    void calcSideVec(TVec3f *pSideVec, const LiveActor *pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        pSideVec->set(mtx[0][0], mtx[1][0], mtx[2][0]);
    }

    void calcPositionUpOffset(TVec3f *a1, const LiveActor *pActor, f32 a3) {
        TVec3f stack_8;
        TVec3f *stack_c = a1;
        MR::calcUpVec(&stack_8, pActor);

        JMAVECScaleAdd((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)stack_c, a3);
    }

    void calcVecToPlayerH(TVec3f *pToPlayerHVec, const LiveActor *pActor, const TVec3f *a3) {
        calcVecToTargetPosH(pToPlayerHVec, pActor, *MR::getPlayerPos(), a3);
    }

    void calcVecFromPlayerH(TVec3f *pFromPlayerHVec, const LiveActor *pActor) {
        calcVecToTargetPosH(pFromPlayerHVec, pActor, *MR::getPlayerPos(), nullptr);
        pFromPlayerHVec->scale(-1.0f);
    }

    void calcVecToTargetPosH(TVec3f *pToTargetHVec, const LiveActor *pActor, const TVec3f &a3, const TVec3f *a4) {
        pToTargetHVec->set<f32>(a3);

        TVec3f *vec = pToTargetHVec;
        PSVECSubtract((Vec*)vec, (Vec*)&pActor->mPosition, (Vec*)vec);

        if (a4 == nullptr) {
            MR::vecKillElement(*pToTargetHVec, pActor->mGravity, pToTargetHVec);
        }
        else {
            MR::vecKillElement(*pToTargetHVec, *a4, pToTargetHVec);
        }

        MR::normalizeOrZero(pToTargetHVec);
    }

    void calcVecFromTargetPosH(TVec3f *pFromTargetHVec, const LiveActor *pActor, const TVec3f &a3, const TVec3f *a4) {
        calcVecToTargetPosH(pFromTargetHVec, pActor, a3, a4);
        pFromTargetHVec->scale(-1.0f);
    }

    bool isFaceToTargetDegree(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a2);

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, a3, a4);
    }

    bool isFaceToPlayerDegree(const LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_8(*MR::getPlayerPos());

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, a2, a3);
    }

    bool isFaceToTargetDegree(const LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8(a2);
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, stack_14, a3);
    }

    bool isFaceToPlayerDegree(const LiveActor *pActor, f32 a2) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8(*MR::getPlayerPos());
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, stack_14, a2);
    }

    bool isFaceToPlayerDegreeHV(const LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4) {
        TVec3f stack_8(*MR::getPlayerPos());

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegreeHV(stack_8, a2, pActor->mGravity, a3, a4);
    }

    bool isFaceToTargetHorizontalDegree(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_20;
        TVec3f stack_14;

        TVec3f stack_8(a2);
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        const TVec3f *grav = &pActor->mGravity;
        JMAVECScaleAdd((Vec*)grav, (Vec*)&stack_8, (Vec*)&stack_20, -pActor->mGravity.dot(stack_8));
        JMAVECScaleAdd((Vec*)grav, (Vec*)&a3, (Vec*)&stack_14, -pActor->mGravity.dot(a3));

        return MR::isNearAngleDegree(stack_20, stack_14, a4);
    }

    bool isFaceToPlayerHorizontalDegree(const LiveActor *pActor, const TVec3f &a2, f32 a3) {
        return MR::isFaceToTargetHorizontalDegree(pActor, *MR::getPlayerPos(), a2, a3);
    }

    bool isClockwiseToDir(const LiveActor *pActor, const TVec3f &v1, const TVec3f &v2) {
        TVec3f stack_8;
        TVec3f stack_14;
        PSVECCrossProduct((Vec*)&pActor->mGravity, (Vec*)&v2, (Vec*)&stack_8);
        normalizeOrZero(stack_8, &stack_14);
        return (stack_14.dot(v1) >= 0.0f);
    }

    bool isClockwiseToPlayer(const LiveActor *pActor, const TVec3f &v1) {
        TVec3f stack_20(*getPlayerPos());
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_20, (Vec*)&pActor->mPosition, (Vec*)&stack_20);
        TVec3f stack_14;
        PSVECCrossProduct((Vec*)&pActor->mGravity, (Vec*)&stack_20, (Vec*)&stack_14);
        TVec3f stack_8;
        normalizeOrZero(stack_14, &stack_8);
        return (stack_8.dot(v1) >= 0.0f);

    }

    // Minor mismatch: The sideVec initialization uses void* TVec3() from Xanimecore
    /* bool isPlayerLeftSide(const LiveActor *pActor) {
        MtxPtr mtx = pActor->getBaseMtx();
        TVec3f sideVec = TVec3f(mtx[0][0], mtx[1][0], mtx[2][0]);
        TVec3f stack_8 = TVec3f(*getPlayerPos());
        JMathInlineVEC::PSVECSubtract(&stack_8, &pActor->mPosition, &stack_8);
        return (stack_8.dot(sideVec) >= 0.0f);
    } */

    bool isBindedWallFront(const LiveActor *pActor, const TVec3f &v1, float a3) {
        if (isBindedWall(pActor)) {
            TVec3f wallNormal(*getWallNormal(pActor)); // stack_8
            if (v1.dot(wallNormal) < -a3) {
                return true;
            }
        }
        return false;
    }

    bool isOnPlayer(const LiveActor *pActor) {
        return isActorOnPlayer(pActor);
    }

    // Minor mismatch: Stack positions of stack_8 (== scaledUpVec) and stack_14 are switched
    /* bool isPlayerExistUp(const LiveActor *pActor, float a2, float a3) {
        TVec3f stack_2c(*getPlayerCenterPos());
        TVec3f upVec; // stack_20
        calcUpVec(&upVec, pActor);
        TVec3f scaledUpVec(upVec); // stack_8
        scaledUpVec.scale(a2);
        TVec3f stack_14(pActor->mPosition);
        stack_14.add(scaledUpVec);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_2c, (Vec*)&stack_14, (Vec*)&stack_2c);
        normalizeOrZero(&stack_2c);
        return (a3 < upVec.dot(stack_2c));
    } */
    
    // Minor mismatch: Stack positions of stack_8 (== scaledDownVec) and stack_14 are switched
    /* bool isExistPlayerDown(const LiveActor *pActor, float a2, float a3) {
        TVec3f stack_38 = TVec3f(*getPlayerCenterPos());
        TVec3f stack_2c;
        calcUpVec(&stack_2c, pActor);
        TVec3f downVec = -stack_2c;
        stack_2c = downVec;
        TVec3f scaledDownVec(stack_2c); // stack_8
        scaledDownVec.scale(a2);
        TVec3f stack_14(pActor->mPosition);
        stack_14.add(scaledDownVec);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_38, (Vec*)&stack_14, (Vec*)&stack_38);
        normalizeOrZero(&stack_38);
        return (a3 < stack_2c.dot(stack_38));
    } */

    bool isInWater(const LiveActor *pActor, const TVec3f &rOffset) {
        TVec3f stack_8(pActor->mPosition);
        stack_8.add(rOffset);

        return MR::isInWater(stack_8);
    }

    bool isInDeath(const LiveActor *pActor, const TVec3f &rOffset) {
        TVec3f stack_8(pActor->mPosition);
        stack_8.add(rOffset);

        return MR::isInDeath(stack_8);
    }

    void makeMtxTR(MtxPtr mtx, const LiveActor *pActor) {
        MR::makeMtxTR(mtx,
            pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z,
            pActor->mRotation.x, pActor->mRotation.y, pActor->mRotation.z);
    }

    void makeMtxTRS(MtxPtr mtx, const LiveActor *pActor) {
        MR::makeMtxTRS(mtx,
            pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z,
            pActor->mRotation.x, pActor->mRotation.y, pActor->mRotation.z,
            pActor->mScale.x, pActor->mScale.y, pActor->mScale.z);
    }

    void makeMtxTransRotateY(MtxPtr mtx, const LiveActor *pActor) {
        MR::makeMtxTransRotateY(mtx,
            pActor->mPosition.x, pActor->mPosition.y, pActor->mPosition.z,
            pActor->mRotation.y);
    }

    void calcMtxFromGravityAndZAxis(TPos3f *pResult, const LiveActor *pActor, const TVec3f &vGravity, const TVec3f &zAxis) {
        TPos3f pos; // stack_44
        pos.setInline(pActor->getBaseMtx());
        TVec3f yDir; // stack_38
        TVec3f zDir; // stack_2c
        TVec3f xDir; // stack_20
        TVec3f xDir2; // stack_14
        if (!isNearZero(vGravity, 0.001f)) {
            TVec3f stack_8 = -vGravity;
            yDir = stack_8;
        }
        else {
            pos.getYDir(yDir);
        }
        if (!isNearZero(zAxis, 0.001f)) {
            zDir = zAxis;
        }
        else {
            pos.getZDir(zDir);
        }
        PSVECCrossProduct((Vec*)&zDir, (Vec*)&yDir, (Vec*)&xDir);
        if (isNearZero(xDir,0.001f)) {
            pos.getXDir(xDir2);
            PSVECCrossProduct((Vec*)&xDir2, (Vec*)&yDir, (Vec*)&zDir);
        }
        makeMtxUpFrontPos(pResult, yDir, zDir, pActor->mPosition);
    }

    void calcAndSetBaseMtxFromGravityAndZAxis(LiveActor *pActor, const TVec3f &zAxis) {
        TPos3f stack_8;
        stack_8.identity();
        calcMtxFromGravityAndZAxis(&stack_8, pActor, pActor->mGravity, zAxis);
        setBaseTRMtx(pActor, stack_8);
    }

    void calcActorAxis(TVec3f *outputX, TVec3f *outputY, TVec3f *outputZ, const LiveActor *pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getXDir(*outputX);
        stack_8.getYDir(*outputY);
        stack_8.getZDir(*outputZ);
    }

    void calcActorAxisX(TVec3f *pDest, const LiveActor *pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getXDir(*pDest);
    }

    void calcActorAxisY(TVec3f *pDest, const LiveActor *pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getYDir(*pDest);
    }

    void calcActorAxisZ(TVec3f *pDest, const LiveActor *pActor) {
        TRot3f stack_8;
        stack_8.identity();
        makeMtxTR(stack_8, pActor);
        stack_8.getZDir(*pDest);
    }

    bool faceToVector(TQuat4f *pQuat, TVec3f a2, f32 a3) {
        TVec3f yDir; // stack_8
        pQuat->getYDir(yDir);
        normalizeOrZero(&a2);
        if (vecKillElement(a2, yDir, &a2) > 0.95f) {
            return false;
        }
        else {
            return turnQuatZDirRad(pQuat, *pQuat, a2, a3 * PI_180);
        }
    }
    
    // Minor mismatch: see isPlayerLeftSide
    /* bool faceToPoint(MtxPtr mtx, TVec3f a2, f32 a3) {
        TVec3f stack_14 = TVec3f(mtx[0][3],mtx[1][3], mtx[2][3]);
        JMathInlineVEC::PSVECSubtract((Vec*)&a2, (Vec*)&stack_14, (Vec*)&a2);
        return faceToVector(mtx, a2, a3);
    } */

    void makeQuatFromRotate(TQuat4f *pQuat, const LiveActor *pActor) {
        makeQuatRotateDegree(pQuat, pActor->mRotation);
        return;
    }

    void makeQuatAndFrontFromRotate(TQuat4f *pQuat, TVec3f *pVec, const LiveActor *pActor) {
        makeQuatRotateDegree(pQuat, pActor->mRotation);
        pQuat->getZDir(*pVec);
        return;
    }

    void blendQuatFromGroundAndFront(TQuat4f *a1, const LiveActor *pActor, const TVec3f &a3, f32 a4, f32 a5) {
        MR::blendQuatUpFront(a1, MR::isBindedGround(pActor) ? *MR::getGroundNormal(pActor) : -pActor->mGravity, a3, a4, a5);
    }

    void resetPosition(LiveActor *pActor) {
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

    void resetPosition(LiveActor *pActor, const TVec3f &rPosition) {
        pActor->mPosition.set<f32>(rPosition);

        MR::resetPosition(pActor);
    }

    void resetPosition(LiveActor *pActor, const char *a2) {
        TVec3f pos(0.0f, 0.0f, 0.0f);
        TVec3f rot(0.0f, 0.0f, 0.0f);
        MR::findNamePos(a2, &pos, &rot);

        pActor->mPosition.set<f32>(pos);
        pActor->mRotation.set<f32>(rot);

        MR::resetPosition(pActor);
    }
    
    void calcVelocityMoveToTarget(TVec3f *a1, const LiveActor *pActor, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        calcVelocityMoveToDirection(a1, pActor, stack_8, a4);
    }

    void addVelocityMoveToDirection(LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, a2, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityMoveToTarget(LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_14(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_14, (Vec*)&(pActor->mPosition), (Vec*)&stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityMoveToTarget(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_14, (Vec*)&(pActor->mPosition), (Vec*)&stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3, a4, a5, a6);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityAwayFromTarget(LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_14(pActor->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_14, (Vec*)&a2, (Vec*)&stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityAwayFromTarget(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14(pActor->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_14, (Vec*)&a2, (Vec*)&stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3, a4, a5, a6);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityClockwiseToDirection(LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_14;
        if (normalizeOrZero(a2, &stack_14)) {
            return;
        }
        PSVECCrossProduct((Vec*)&(pActor->mGravity), (Vec*)&stack_14, (Vec*)&stack_14);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, stack_14, a3);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityClockwiseToTarget(LiveActor *pActor, const TVec3f &a2, f32 a3) {
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        addVelocityClockwiseToDirection(pActor, stack_8, a3);
    }

    void addVelocityClockwiseToPlayer(LiveActor *pActor, f32 a2) {
        TVec3f stack_8(*getPlayerPos());
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        addVelocityClockwiseToDirection(pActor, stack_8, a2);
    }

    void addVelocityJump(LiveActor *pActor, f32 a2) {
        TVec3f stack_8(pActor->mGravity);
        stack_8.scale(-a2);
        (pActor->mVelocity).add(stack_8);
    }

    void setVelocityJump(LiveActor *pActor, f32 a2) {
        TVec3f stack_8(pActor->mGravity);
        stack_8.scale(-a2);
        (pActor->mVelocity).set(stack_8);
    }

    void addVelocityToGravity(LiveActor *pActor, f32 a2) {
        TVec3f stack_8(pActor->mGravity);
        stack_8.scale(a2);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocityToGravityOrGround(LiveActor *pActor, f32 a2) {
        if (isBindedGround(pActor)) {
            JMAVECScaleAdd((Vec*)getGroundNormal(pActor), (Vec*)&(pActor->mVelocity), (Vec*)&(pActor->mVelocity), -a2);
        }
        else {
            addVelocityToGravity(pActor,a2);
        }
    }
    
    bool addVelocityToCollisionNormal(LiveActor *pActor, f32 a2) {
        if (isBinded(pActor) == false) {
            return false;
        }
        else {
            TVec3f stack_14(*getBindedFixReactionVector(pActor));
            if (isNearZero(stack_14, 0.001f)) {
                return false;
            }
            normalize(&stack_14);
            TVec3f stack_8(stack_14);
            stack_8.scale(a2);
            (pActor->mVelocity).add(stack_8);
            return true;
        }
    }

    void addVelocityKeepHeight(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4) {
        addVelocityKeepHeight(pActor, a2, 0.0f, a3, a4);
    }

    void addVelocityKeepHeight(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4, f32 a5) {
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        addVelocityToGravity(pActor, a4 * normalizeAbs((pActor->mGravity).dot(stack_8) - a3, -a5, a5));
    }
    
    // Minor mismatch: Two adjacent instructions get swapped
    /* bool addVelocityKeepHeightUseShadow(LiveActor *pActor, f32 a2, f32 a3, f32 a4, const char *pName) {
        if (pName != nullptr) {
            if (isShadowProjected(pActor, pName) == false) {
                return false;
            }
        }
        else {
            if (isShadowProjectedAny(pActor) == false) {
                return false;
            }
        }
        addVelocityToGravity(pActor, a3 * normalizeAbs(((pName != nullptr) ? getShadowProjectionLength(pActor, pName) : getShadowNearProjectionLength(pActor)) - a2, -a4, a4));
        return true;
    } */

    void attenuateVelocity(LiveActor *pActor, f32 scalar) {
        (pActor->mVelocity).scale(scalar);
    }

    void restrictVelocity(LiveActor *pActor, f32 speed) {
        if ((pActor->mVelocity).squared() > speed * speed) {
            (pActor->mVelocity).setLength(speed);
        }
    }

    void zeroVelocity(LiveActor *pActor) {
        (pActor->mVelocity).zero();
    }

    void setVelocity(LiveActor *pActor, const TVec3f &speed) {
        (pActor->mVelocity).set(speed);
    }

    void addVelocity(LiveActor *pActor, const TVec3f &speed) {
        (pActor->mVelocity).add(speed);
    }

    void scaleVelocity(LiveActor *pActor, f32 scaleFactor) {
        (pActor->mVelocity).scale(scaleFactor);
    }

    f32 calcVelocityLength(const LiveActor *pActor) {
        return PSVECMag((Vec*)&(pActor->mVelocity));
    }

    f32 calcGravitySpeed(const LiveActor *pActor) {
        return (pActor->mVelocity).dot(pActor->mGravity);
    }

    // 80.24% match
    /* void applyVelocityDampAndGravity(LiveActor *pActor, f32 a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_38;
        TVec3f stack_2c;
        if (isBindedGround(pActor) == false) {
            TVec3f stack_14(pActor->mGravity);
            stack_14.scale(a2);
            pActor->mVelocity.add(stack_14);
        }
        JMAVECScaleAdd((Vec*)&(pActor->mGravity), (Vec*)&(pActor->mVelocity), (Vec*)&stack_38, -pActor->mGravity.dot(pActor->mVelocity));
        stack_2c.scale(pActor->mGravity.dot(pActor->mVelocity), pActor->mGravity);
        if (isOnGround(pActor)) {
            stack_38.scale(a3);
        }
        else {
            stack_38.scale(a4);
        }
        if (stack_2c.dot(pActor->mGravity) < 0.0f) {
            stack_2c.scale(a5);
        }
        TVec3f stack_8(stack_38);
        stack_8.add(stack_2c);
        pActor->mVelocity.set(stack_8);
        if (isOnGround(pActor)) {
            TVec3f stack_20(*getGroundNormal(pActor));
            JMAVECScaleAdd((Vec*)&stack_20, (Vec*)&pActor->mVelocity, (Vec*)&stack_38, -stack_20.dot(pActor->mVelocity));
            if (stack_38.squared() < a6 * a6) {
                pActor->mVelocity.scale(stack_20.dot(pActor->mVelocity), stack_20);
            }
        }
    } */

    // Minor mismatch: Velocity is loaded each time it is used as "pActor offset 0x3c" rather than storing that address in a register and loading from that register each time
    /* void setVelocityJumpAwayFromPlayer(LiveActor *pActor, f32 a2, f32 a3) {
        calcVecToTargetPosH(&(pActor->mVelocity), (pActor), *getPlayerPos(), nullptr);
        (pActor->mVelocity).scale(-1.0f);
        (pActor->mVelocity).scale(a2);
        addVelocityJump(pActor, a3);
    } */

    // Minor mismatch: See setVelocityJumpAwayFromPlayer
    /* bool sendMsgPushAndKillVelocityToTarget(LiveActor *pActor, HitSensor *pSensor1, HitSensor *pSensor2) {
        if (sendMsgPush(pSensor1, pSensor2)) {
            TVec3f stack_8(pSensor2->mPosition);
            JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pSensor1->mPosition), (Vec*)&stack_8);
            normalizeOrZero(&stack_8);
            if (pActor->mVelocity.dot(stack_8) < 0.0f) {
                JMAVECScaleAdd((Vec*)&stack_8, (Vec*)&(pActor->mVelocity), (Vec*)&(pActor->mVelocity), -stack_8.dot(pActor->mVelocity));
            }
            return true;
        }
        return false;

    } */

    void addVelocityFromPush(LiveActor *pActor, f32 a2, HitSensor *pSensor1, HitSensor *pSensor2) {
        TVec3f stack_14(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_14, (Vec*)&(pSensor1->mPosition), (Vec*)&stack_14);
        TVec3f stack_8 = -(pActor->mGravity);
        if (a2 < (pActor->mVelocity).dot(stack_8)) {
            vecKillElement(stack_14, pActor->mGravity, &stack_14);
        }
        stack_14.setLength(a2);
        addVelocityLimit(pActor, stack_14);
    }

    void addVelocityFromPushHorizon(LiveActor *pActor, f32 speed, HitSensor *pSensor1, HitSensor *pSensor2) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pSensor1->mPosition), (Vec*)&stack_8);
        vecKillElement(stack_8, pActor->mGravity, &stack_8);
        stack_8.setLength(speed);
        (pActor->mVelocity).add(stack_8);
    }

    void addVelocitySeparateHV(LiveActor *pActor, HitSensor *pSensor1, HitSensor *pSensor2, f32 a4, f32 a5) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pSensor1->mPosition), (Vec*)&stack_8);
        addVelocitySeparateHV(pActor, stack_8, a4, a5);
    }

    void setVelocitySeparateHV(LiveActor *pActor, HitSensor *pSensor1, HitSensor *pSensor2, f32 a4, f32 a5) {
        TVec3f stack_8(pSensor2->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pSensor1->mPosition), (Vec*)&stack_8);
        setVelocitySeparateHV(pActor, stack_8, a4, a5);
    }

    void setVelocityBlowAttack(LiveActor *pActor, HitSensor *pSensor1, HitSensor *pSensor2, f32 a4, f32 a5, s32 a6) {
        setVelocitySeparateHV(pActor, pSensor1, pSensor2, a4, a5);
        if (isNearPlayerAnyTime(pActor, 1000.0f)) {
            stopSceneForDefaultHit(a6);
        }
    }

    // Minor mismatch: f1 used instead of f31 in the float compare
    /* bool reflectVelocityH(LiveActor *pActor, HitSensor *pSensor1, HitSensor *pSensor2, f32 a4) {
        TVec3f stack_14;
        calcSensorHorizonNormalize(&stack_14, (pActor->mGravity), pSensor2, pSensor1);
        if (((1.0f + a4) * pActor->mVelocity.dot(stack_14)) > 0.0f) {
            return false;
        }
        TVec3f stack_8(stack_14);
        stack_8.scale(a4);
        JMathInlineVEC::PSVECSubtract((Vec*)&(pActor->mVelocity), (Vec*)&stack_8, (Vec*)&(pActor->mVelocity));
        return true;
    } */

    bool calcVelocityAreaMoveOnGround(TVec3f *a1, const LiveActor *pActor) {
        a1->zero();
        if ((isOnGround(pActor) == false) || (isBindedGroundAreaMove(pActor) == false)) {
            return false;
        }
        calcAreaMoveVelocity(a1, pActor->mPosition);
        return true;
    }

    void rotateDirectionGravityDegree(const LiveActor *pActor, TVec3f *a2, f32 angle) {
        rotateVecRadian(a2, (pActor->mGravity), PI_180 * angle);
    }

    void turnDirection(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        turnVecToVecCosOnPlane(a2, a3, (pActor->mGravity), a4);
    }

    void turnDirectionDegree(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        turnVecToVecCosOnPlane(a2, a3, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionToTarget(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, a4);
    }

    void turnDirectionToTargetDegree(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionToTargetDegreeHorizon(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        vecKillElement(stack_8, pActor->mGravity, &stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, cosDegree(a4));
    }

    void turnDirectionToTargetUseGroundNormalDegree(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        a4 = cosDegree(a4);
        turnVecToVecCosOnPlane(a2, stack_8, isBindedGround(pActor) ? *getGroundNormal(pActor) : (pActor->mGravity), a4);
    }

    void turnDirectionToPlayerDegree(const LiveActor *pActor, TVec3f *a2, f32 a3) {
        turnDirectionToTargetDegree(pActor, a2, *getPlayerPos(), a3);
    }
    
    void turnDirectionToPlayerDegreeHorizon(const LiveActor *pActor, TVec3f *a2, f32 a3) {
        turnDirectionToTargetDegreeHorizon(pActor, a2, *getPlayerPos(), a3);
    }

    void turnDirectionFromTargetDegree(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(pActor->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&a3, (Vec*)&stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, pActor->mGravity, cosDegree(a4));
    }

    void turnDirectionFromTargetDegreeHorizon(const LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4) {
        TVec3f stack_8(pActor->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&a3, (Vec*)&stack_8);
        vecKillElement(stack_8, (pActor->mGravity), &stack_8);
        turnVecToVecCosOnPlane(a2, stack_8, (pActor->mGravity), cosDegree(a4));
    }

    void turnDirectionFromPlayerDegree(const LiveActor *pActor, TVec3f *a2, f32 a3) {
        turnDirectionFromTargetDegree(pActor, a2, *getPlayerPos(), a3);
    }

    void turnDirectionFromPlayerDegreeHorizon(const LiveActor *pActor, TVec3f *a2, f32 a3) {
        turnDirectionFromTargetDegreeHorizon(pActor, a2, *getPlayerPos(), a3);
    }

    // Moderate mismatch: Bad instruction order
    /* void turnDirectionAndGravityH(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4) {
        TQuat4f stack_8;
        makeQuatRotateDegree(&stack_8, pActor->mRotation);
        turnDirectionAndGravityH(&stack_8, pActor, a2, a3, a4);
        stack_8.getEuler(pActor->mRotation);
        pActor->mRotation.set( 57.29578f * (pActor->mRotation).x, 57.29578f * (pActor->mRotation).y, 57.29578f * (pActor->mRotation).z);
    } */

    void turnDirectionToGround(const LiveActor *pActor, TVec3f *a2) {
        
        TVec3f stack_8;
        if (isBindedGround(pActor)) {
            stack_8.scale(-1.0f, *getGroundNormal(pActor));
        }
        else {
            stack_8.set(pActor->mGravity);
        }
        JMAVECScaleAdd((Vec*)&stack_8, (Vec*)a2, (Vec*)a2, -stack_8.dot(*a2));
        normalize(a2);
    }
    
    void moveAndTurnToDirection(LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        turnVecToVecCosOnPlane(a2, a3, pActor->mGravity, cosDegree(a7));
        turnDirectionToGround(pActor, a2);
        TVec3f stack_8;
        calcVelocityMoveToDirection(&stack_8, pActor, *a2, a4);
        pActor->mVelocity.add(stack_8);
        pActor->mVelocity.scale(a6);
        reboundVelocityFromCollision(pActor, 0.0f, 0.0f, 1.0f);
        addVelocityToGravityOrGround(pActor, a5);
    }

    void moveAndTurnToTarget(LiveActor *pActor, TVec3f *a2, const TVec3f &a3, f32 a4, f32 a5, f32 a6, f32 a7) {
        TVec3f stack_8(a3);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        moveAndTurnToDirection(pActor, a2, stack_8, a4, a5, a6, a7);
    }

    void moveAndTurnToPlayer(LiveActor *pActor, TVec3f *a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8(*getPlayerPos());
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&(pActor->mPosition), (Vec*)&stack_8);
        moveAndTurnToDirection(pActor, a2, stack_8, a3, a4, a5, a6);
    }

    // Minor mismatch: getPlayerPos() is called after stack_8 is initialized instead of before
    /* void escapeFromPlayer(LiveActor *pActor, TVec3f *a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8(pActor->mPosition);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)getPlayerPos(), (Vec*)&stack_8);
        moveAndTurnToDirection(pActor, a2, stack_8, a3, a4, a5, a6);
    } */

    void moveAndTurnToDirection(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_8;
        calcFrontVec(&stack_8, pActor);
        moveAndTurnToDirection(pActor, &stack_8, a2, a3, a4, a5, a6);
        calcRotate(pActor, stack_8, a6);
    }

    void moveAndTurnToTarget(LiveActor *pActor, const TVec3f &a2, f32 a3, f32 a4, f32 a5, f32 a6) {
        TVec3f stack_14;
        calcFrontVec(&stack_14, pActor);
        normalizeOrZero(&stack_14);
        TVec3f stack_8(a2);
        JMathInlineVEC::PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);
        moveAndTurnToDirection(pActor, &stack_14, stack_8, a3, a4, a5, a6);
        calcRotate(pActor, stack_14, a6);
    }

    void moveAndTurnToPlayer(LiveActor *pActor, f32 a3, f32 a4, f32 a5, f32 a6) {
        moveAndTurnToTarget(pActor, *getPlayerPos(), a3, a4, a5, a6);
    }
};

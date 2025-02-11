#include "Game/Util.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ShadowController.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "JSystem/JGeometry.hpp"
#include "JSystem/JMath/JMath.hpp"
#include "math_types.hpp"

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

    // MR::isNearPlayerPose((LiveActor const *, float, float))
    // MR::isNearPlayerHorizontal((LiveActor const *, float))

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

    bool isFaceToTargetDegree(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, float a4) {
        TVec3f stack_8(a2);

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, a3, a4);
    }

    bool isFaceToPlayerDegree(const LiveActor *pActor, const TVec3f &a2, float a3) {
        TVec3f stack_8(*MR::getPlayerPos());

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, a2, a3);
    }

    bool isFaceToTargetDegree(const LiveActor *pActor, const TVec3f &a2, float a3) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8(a2);
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, stack_14, a3);
    }

    bool isFaceToPlayerDegree(const LiveActor *pActor, float a2) {
        TVec3f stack_14;
        MR::calcFrontVec(&stack_14, pActor);

        TVec3f stack_8(*MR::getPlayerPos());
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegree(stack_8, stack_14, a2);
    }

    bool isFaceToPlayerDegreeHV(const LiveActor *pActor, const TVec3f &a2, float a3, float a4) {
        TVec3f stack_8(*MR::getPlayerPos());

        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        return MR::isNearAngleDegreeHV(stack_8, a2, pActor->mGravity, a3, a4);
    }

    bool isFaceToTargetHorizontalDegree(const LiveActor *pActor, const TVec3f &a2, const TVec3f &a3, float a4) {
        TVec3f stack_20;
        TVec3f stack_14;

        TVec3f stack_8(a2);
        PSVECSubtract((Vec*)&stack_8, (Vec*)&pActor->mPosition, (Vec*)&stack_8);

        const TVec3f *grav = &pActor->mGravity;
        JMAVECScaleAdd((Vec*)grav, (Vec*)&stack_8, (Vec*)&stack_20, -pActor->mGravity.dot(stack_8));
        JMAVECScaleAdd((Vec*)grav, (Vec*)&a3, (Vec*)&stack_14, -pActor->mGravity.dot(a3));

        return MR::isNearAngleDegree(stack_20, stack_14, a4);
    }

    bool isFaceToPlayerHorizontalDegree(const LiveActor *pActor, const TVec3f &a2, float a3) {
        return MR::isFaceToTargetHorizontalDegree(pActor, *MR::getPlayerPos(), a2, a3);
    }

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

    void blendQuatFromGroundAndFront(TQuat4f *a1, const LiveActor *pActor, const TVec3f &a3, float a4, float a5) {
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
};

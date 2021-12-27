#include "Game/Util.h"
#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/HitSensor.h"
#include "Game/Scene/SceneObjHolder.h"
#include "JSystem/JMath/JMath.h"
#include "math_types.h"

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

    // MR::calcDistanceVertical((LiveActor const *, JGeometry::TVec3<f> const &))

    f32 calcDistanceToPlayer(const LiveActor *pActor) {
        return (!MR::isExistSceneObj(SceneObj_MarioHolder) ? FLOAT_MAX : PSVECDistance((Vec*)&pActor->mPosition, (Vec*)MR::getPlayerPos()));
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
};
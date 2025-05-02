#include "Game/Enemy/SpinHitController.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

SpinHitController::SpinHitController(const NameObj *pObj, s32 a1, bool a2) {
    setParameter(pObj, 15, 10, 3, 0.0f, 40.0f, 20.0f, a1, a2);
}

SpinHitController::SpinHitController(const NameObj *pObj, s32 a2, s32 a3, s32 a4, f32 a5, f32 a6, f32 a7, s32 a8, bool a9) {
    setParameter(pObj, a2, a3, a4, a5, a6, a7, a8, a9);
}

void SpinHitController::setParameter(const NameObj *pObj, s32 a2, s32 a3, s32 a4, f32 a5, f32 a6, f32 a7, s32 a8, bool a9) {
    _0 = a2;
    _4 = a3;
    _8 = a4;
    _C = a5;
    _10 = a6;
    _14 = a7;
    _19 = a9;
    _1C = a8;
}

void SpinHitController::start(LiveActor *pActor, const TVec3f &a2, const TVec3f &a3) {
    TVec3f v9(a3);
    v9 -= a2;
    MR::vecKillElement(v9, pActor->mGravity, &v9);
    MR::normalizeOrZero(&v9);
    v9 *= _10;
    f32 val = _14;
    TVec3f v8(pActor->mGravity);
    v8 *= val;
    v9 -= v8;
    pActor->mVelocity.x = v9.x;
    pActor->mVelocity.y = v9.y;
    pActor->mVelocity.z = v9.z;
    _18 = true;
}

void SpinHitController::startWithoutStopScene(LiveActor *pActor, const TVec3f &a2, const TVec3f &a3) {
    start(pActor, a2, a3);
    _18 = false;
}

bool SpinHitController::execute(LiveActor *pActor) {
    if (MR::isFirstStep(pActor)) {
        MR::invalidateClipping(pActor);
        
        if (_18) {
            MR::stopSceneForDefaultHit(_8);
        }
    }

    f32 val = _C;
    TVec3f v12(pActor->mGravity);
    v12 *= val;
    JMathInlineVEC::PSVECAdd(&pActor->mVelocity, &v12, &pActor->mVelocity);

    if (MR::isStep(pActor, _4)) {
        emitItem(pActor);
    }

    if ((MR::isBinded(pActor) && MR::isGreaterStep(pActor, 1)) || MR::isGreaterStep(pActor, _0)) {
        MR::validateClipping(pActor);
        pActor->kill();
        if (MR::isLessStep(pActor, _4)) {
            emitItem(pActor);
        }
    
        return true;
    }

    return false;
}

void SpinHitController::emitItem(LiveActor *pActor) {
    if (_1C != 0) {
        if (_19) { 
            MR::appearCoinPop(pActor, pActor->mPosition, _1C);
        }
        else {
            MR::appearStarPiece(pActor, pActor->mPosition, _1C, 10.0f, 40.0f, false);
            if (pActor->mSoundObject != nullptr) {
                MR::startSound(pActor, "SE_OJ_STAR_PIECE_BURST", -1, -1);
            }
        }
        
        _1C = 0;
    }
}

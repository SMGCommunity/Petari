#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossBreakMovement.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JMath.hpp>

TripodBossFixPartsBase::TripodBossFixPartsBase(const char *pName) : LiveActor(pName) {
    _BC.x = 0.0f;
    _BC.y = 0.0f;
    _BC.z = 0.0f;
    mBreakMovement = nullptr;
    _CC = -1;
    _D0.x = 0.0f;
    _D0.y = 0.0f;
    _D0.z = 0.0f;
    mClippingDistance = 0.0f;
    _E0 = 0;
    _E1 = 0;
    _8C.identity();
}

void TripodBossFixPartsBase::init(const JMapInfoIter &rIter) {
    MR::initDefaultPos(this, rIter);
    _BC = mPosition;
    MR::getJMapInfoArg0NoInit(rIter, &_CC);
    MR::addTripodBossParts(this);
    MR::invalidateClipping(this);
}

void TripodBossFixPartsBase::initAfterPlacement() {
    updateTripodMatrix();
    calcAnim();
    MR::offCalcAnim(this);
}

void TripodBossFixPartsBase::initClippingSphere() {
    TBox3f box;
    MR::calcModelBoundingBox(&box, this);
    TVec3f v9(box.mMin);
    JMathInlineVEC::PSVECAdd(&v9, &box.mMax, &v9);
    TVec3f v10(v9);
    v10.x *= 0.5f;
    v10.y *= 0.5f;
    v10.z *= 0.5f;
    _D0 = v10;
    mClippingDistance = PSVECDistance(&_D0, &box.mMin);
}

void TripodBossFixPartsBase::setClippingSphere(const TVec3f &a1, f32 dist) {
    _D0 = a1;
    mClippingDistance = dist;
}

void TripodBossFixPartsBase::makeActorDead() {
    LiveActor::makeActorDead();
    if (mBreakMovement != nullptr) {
        mBreakMovement->makeActorDead();
    }
}

void TripodBossFixPartsBase::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _8C);
}

bool TripodBossFixPartsBase::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    if (msg == 170) {
        activateTripodBoss();
        return true;
    }

    return false;
}

void TripodBossFixPartsBase::calcTripodLocalMatrix(TPos3f *pPos) {
    MR::makeMtxTR(*pPos, _BC, mRotation);
}

void TripodBossFixPartsBase::activateTripodBoss() {

}

void TripodBossFixPartsBase::startBreakMovement() {
    TPos3f mtx;
    mtx.setInline(getBaseMtx());
    mBreakMovement->start(mtx, _CC);
}

/* matrix copy is wrong */
void TripodBossFixPartsBase::updateBreakMovementMatrix() {
    mBreakMovement->movement();
    _8C = mBreakMovement->_8C;
    _8C.getTrans(mPosition);
}

void TripodBossFixPartsBase::updateTripodMatrix() {
    TVec3f mul;
    calcTripodLocalMatrix(&_8C);
    MR::concatTripodBossAttachJointMatrix(&_8C, _CC);
    _8C.getTrans(mPosition);

    if (_E0) {
        _8C.mult(_D0, mul);
        if (MR::isJudgedToClipFrustum(mul, mClippingDistance)) {
            MR::hideModelAndOnCalcAnim(this);
        }
        else {
            MR::showModel(this);
        }
    }
}

void TripodBossFixPartsBase::initBreakMovement(s32 level) {
    mBreakMovement = new TripodBossBreakMovement("三脚ボスパーツ破壊挙動");
    mBreakMovement->setBreakDownLevel(level);
}

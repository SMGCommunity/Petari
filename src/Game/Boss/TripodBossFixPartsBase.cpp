#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Boss/TripodBossBreakMovement.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include <JSystem/JMath.hpp>

TripodBossFixPartsBase::TripodBossFixPartsBase(const char* pName)
    : LiveActor(pName), _BC(0.0f, 0.0f, 0.0f), mBreakMovement(), mJointID(-1), _D0(0.0f, 0.0f, 0.0f), mClippingRadius(), _E0(), _E1() {
    _8C.identity();
}

void TripodBossFixPartsBase::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    _BC = mPosition;
    MR::getJMapInfoArg0NoInit(rIter, &mJointID);
    MR::addTripodBossParts(this);
    MR::invalidateClipping(this);
}

void TripodBossFixPartsBase::initAfterPlacement() {
    updateTripodMatrix();
    calcAnim();
    MR::offCalcAnim(this);
}

void TripodBossFixPartsBase::initClippingSphere() {
    TBox3f boundingBox;
    MR::calcModelBoundingBox(&boundingBox, this);

    _D0 = (boundingBox.i + boundingBox.f) * 0.5f;
    mClippingRadius = _D0.distance(boundingBox.i);
}

void TripodBossFixPartsBase::setClippingSphere(const TVec3f& a1, f32 radius) {
    _D0 = a1;
    mClippingRadius = radius;
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

bool TripodBossFixPartsBase::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_TRIPODBOSS_STARTED) {
        activateTripodBoss();

        return true;
    }

    return false;
}

void TripodBossFixPartsBase::calcTripodLocalMatrix(TPos3f* pMtx) {
    MR::makeMtxTR(*pMtx, _BC, mRotation);
}

void TripodBossFixPartsBase::activateTripodBoss() {
}

void TripodBossFixPartsBase::startBreakMovement() {
    TPos3f mtx;
    mtx.set(getBaseMtx());

    mBreakMovement->start(mtx, mJointID);
}

void TripodBossFixPartsBase::updateBreakMovementMatrix() {
    mBreakMovement->movement();

    _8C = mBreakMovement->_8C;
    _8C.getTrans(mPosition);
}

void TripodBossFixPartsBase::updateTripodMatrix() {
    calcTripodLocalMatrix(&_8C);
    MR::concatTripodBossAttachJointMatrix(&_8C, mJointID);
    _8C.getTrans(mPosition);

    if (!_E0) {
        return;
    }

    TVec3f mul;
    _8C.mult(_D0, mul);

    if (MR::isJudgedToClipFrustum(mul, mClippingRadius)) {
        MR::hideModelAndOnCalcAnim(this);
    } else {
        MR::showModel(this);
    }
}

void TripodBossFixPartsBase::initBreakMovement(s32 level) {
    mBreakMovement = new TripodBossBreakMovement("三脚ボスパーツ破壊挙動");
    mBreakMovement->setBreakDownLevel(level);
}

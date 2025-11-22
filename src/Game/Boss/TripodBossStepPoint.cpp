#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util/JMapUtil.hpp"

TripodBossStepPoint::TripodBossStepPoint(const char* pName)
    : LiveActor(pName), mStepPosition(0, 0, 0), mStepNormal(0, 1, 0), mStepFront(0, 0, 1) {
    mLeg = 0;
    _B4 = 0;
    _B8 = -1;
    mWaitTime = 120;
}

void TripodBossStepPoint::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        TPos3f mtx;
        MR::getJMapInfoMatrixFromRT(rIter, &mtx);
        mStepPosition.set< f32 >(mtx(0, 3), mtx(1, 3), mtx(2, 3));
        mStepNormal.set< f32 >(mtx(0, 1), mtx(1, 1), mtx(2, 1));
        mStepFront.set< f32 >(mtx(0, 2), mtx(1, 2), mtx(2, 2));
        MR::getJMapInfoArg0NoInit(rIter, &_B4);
        MR::getJMapInfoArg1NoInit(rIter, &mLeg);
        MR::getJMapInfoArg2NoInit(rIter, &mWaitTime);
        MR::getJMapInfoArg3NoInit(rIter, &_B8);
    }

    makeActorDead();
}

void TripodBossStepPoint::initAfterPlacement() {
    if (_B8 != -1) {
        MR::addTripodBossStepPoint(this);
    }
}

void TripodBossStepPoint::setStepPosition(const TVec3f& rPos) {
    mStepPosition = rPos;
}

void TripodBossStepPoint::setStepNormal(const TVec3f& rNorm) {
    mStepNormal = rNorm;
    MR::normalizeOrZero(&mStepNormal);
}

void TripodBossStepPoint::setStepFront(const TVec3f& rFront) {
    mStepFront = rFront;
    MR::normalizeOrZero(&mStepFront);
}

TripodBossStepPoint::~TripodBossStepPoint() {
}

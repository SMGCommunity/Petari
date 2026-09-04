#include "Game/Boss/TripodBossStepPoint.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/MathUtil.hpp"

TripodBossStepPoint::TripodBossStepPoint(const char* pName)
    : LiveActor(pName), mStepPosition(0, 0, 0), mStepNormal(0, 1, 0), mStepFront(0, 0, 1), mLeg(), mSequencePriority(), mArg3(-1), mWaitTime(120) {
}

void TripodBossStepPoint::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        TPos3f mtx;
        MR::getJMapInfoMatrixFromRT(rIter, &mtx);

        mtx.getTrans(mStepPosition);
        mtx.getYDir(mStepNormal);
        mtx.getZDir(mStepFront);

        MR::getJMapInfoArg0NoInit(rIter, &mSequencePriority);
        MR::getJMapInfoArg1NoInit(rIter, &mLeg);
        MR::getJMapInfoArg2NoInit(rIter, &mWaitTime);
        MR::getJMapInfoArg3NoInit(rIter, &mArg3);
    }

    makeActorDead();
}

void TripodBossStepPoint::initAfterPlacement() {
    if (mArg3 != -1) {
        MR::addTripodBossStepPoint(this);
    }
}

void TripodBossStepPoint::setStepPosition(const TVec3f& rPos) {
    mStepPosition = rPos;
}

void TripodBossStepPoint::setStepNormal(const TVec3f& rNormal) {
    mStepNormal = rNormal;

    MR::normalizeOrZero(&mStepNormal);
}

void TripodBossStepPoint::setStepFront(const TVec3f& rFront) {
    mStepFront = rFront;

    MR::normalizeOrZero(&mStepFront);
}

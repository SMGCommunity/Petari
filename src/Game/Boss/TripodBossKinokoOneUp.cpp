#include "Game/Boss/TripodBossKinokoOneUp.hpp"
#include "Game/Boss/TripodBossAccesser.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"

namespace NrvTripodBossKinokoOneUp {
    NEW_NERVE(TripodBossKinokoOneUpNrvActive, TripodBossKinokoOneUp, Active);
    NEW_NERVE(TripodBossKinokoOneUpNrvEnd, TripodBossKinokoOneUp, End);
};  // namespace NrvTripodBossKinokoOneUp

TripodBossKinokoOneUp::TripodBossKinokoOneUp(const char* pName) : LiveActor(pName), mOneUp(), mJointID(-1) {
    _8C.identity();
    _BC.identity();
}

void TripodBossKinokoOneUp::init(const JMapInfoIter& rIter) {
    MR::getJMapInfoMatrixFromRT(rIter, &_8C);
    MR::connectToSceneMapObjDecorationMovement(this);
    MR::invalidateClipping(this);
    MR::getJMapInfoArg0NoInit(rIter, &mJointID);
    initNerve(&NrvTripodBossKinokoOneUp::TripodBossKinokoOneUpNrvActive::sInstance);

    mOneUp = new BenefitItemOneUp("1UPキノコ", "KinokoOneUp");
    mOneUp->setFollowMtx(_BC);
    mOneUp->initWithoutIter();
    MR::invalidateClipping(mOneUp);

    makeActorDead();
    MR::addTripodBossParts(this);
}

void TripodBossKinokoOneUp::control() {
}

void TripodBossKinokoOneUp::exeActive() {
    _BC.set(_8C);
    MR::concatTripodBossAttachJointMatrix(&_BC, mJointID);
    _BC.getTrans(mPosition);

    if (MR::isFirstStep(this)) {
        mOneUp->makeActorAppeared();
    }

    if (MR::isBrokenTripodBoss() || MR::isDead(mOneUp)) {
        mOneUp->kill();
        setNerve(&NrvTripodBossKinokoOneUp::TripodBossKinokoOneUpNrvEnd::sInstance);
    }
}

void TripodBossKinokoOneUp::exeEnd() {
}

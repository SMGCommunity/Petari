#include "Game/Boss/TombSpiderVitalSpot.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/LiveActorUtil.hpp"

namespace NrvTombSpiderVitalSpot {
    NEW_NERVE(TombSpiderVitalSpotNrvWait, TombSpiderVitalSpot, Wait);
    NEW_NERVE(TombSpiderVitalSpotNrvChanceWait, TombSpiderVitalSpot, ChanceWait);
    NEW_NERVE(TombSpiderVitalSpotNrvDamageStart, TombSpiderVitalSpot, DamageStart);
    NEW_NERVE(TombSpiderVitalSpotNrvDamageWait, TombSpiderVitalSpot, DamageWait);
    NEW_NERVE(TombSpiderVitalSpotNrvRecover, TombSpiderVitalSpot, Recover);
}  // namespace NrvTombSpiderVitalSpot

TombSpiderVitalSpot::TombSpiderVitalSpot(LiveActor* pActor, const char* pName, const TVec3f& rPos, const TVec3f& a1, const char* pJointName)
    : PartsModel(pActor, pName, "TombSpiderVitalSpot", nullptr, MR::DrawBufferType_Enemy, false) {
    MR::initLightCtrl(this);
    initFixedPosition(rPos, a1, pJointName);
}

void TombSpiderVitalSpot::init(const JMapInfoIter& rIter) {
    PartsModel::init(rIter);
    initNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvWait::sInstance);
    makeActorAppeared();
}

void TombSpiderVitalSpot::startChance() {
    if (isNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvWait::sInstance)) {
        setNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvChanceWait::sInstance);
    }
}

void TombSpiderVitalSpot::endChance() {
    if (isNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvChanceWait::sInstance)) {
        setNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvWait::sInstance);
    }
}

void TombSpiderVitalSpot::startDamage() {
    setNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvDamageStart::sInstance);
}

void TombSpiderVitalSpot::recover() {
    setNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvRecover::sInstance);
}

bool TombSpiderVitalSpot::isDamage() const {
    if (isNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvDamageStart::sInstance) ||
        isNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvDamageWait::sInstance)) {
        return true;
    }
    return false;
}

void TombSpiderVitalSpot::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void TombSpiderVitalSpot::exeChanceWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "ChanceWait", nullptr);
        MR::startBrk(this, "TombSpiderVitalSpot");
    }
}

void TombSpiderVitalSpot::exeDamageStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DamageStart", nullptr);
        MR::startBrk(this, "TombSpiderVitalSpot");
        MR::startSound(this, "SE_BM_TSPIDER_DAMAGE_VITAL", -1, -1);
    }
}

void TombSpiderVitalSpot::exeDamageWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DamageWait", nullptr);
    }
}

void TombSpiderVitalSpot::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DamageEnd", nullptr);
        MR::startBrk(this, "TombSpiderVitalSpot");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTombSpiderVitalSpot::TombSpiderVitalSpotNrvWait::sInstance);
    }
}

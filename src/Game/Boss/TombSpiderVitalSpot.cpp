#include "Game/Boss/TombSpiderVitalSpot.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvTombSpiderVitalSpot {
    NEW_NERVE(TombSpiderVitalSpotNrvWait, TombSpiderVitalSpot, Wait);
    NEW_NERVE(TombSpiderVitalSpotNrvChanceWait, TombSpiderVitalSpot, ChanceWait);
    NEW_NERVE(TombSpiderVitalSpotNrvDamageStart, TombSpiderVitalSpot, DamageStart);
    NEW_NERVE(TombSpiderVitalSpotNrvDamageWait, TombSpiderVitalSpot, DamageWait);
    NEW_NERVE(TombSpiderVitalSpotNrvRecover, TombSpiderVitalSpot, Recover);
};  // namespace NrvTombSpiderVitalSpot

TombSpiderVitalSpot::TombSpiderVitalSpot(LiveActor* pActor, const char* pName, const TVec3f& rPos, const TVec3f& a1, const char* pJointName)
    : PartsModel(pActor, pName, "TombSpiderVitalSpot", nullptr, MR::DrawBufferType_Enemy, false) {
    MR::initLightCtrl(this);
    initFixedPosition(rPos, a1, pJointName);
}

void TombSpiderVitalSpot::init(const JMapInfoIter& rIter) {
    PartsModel::init(rIter);
    initNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvWait));
    makeActorAppeared();
}

void TombSpiderVitalSpot::startChance() {
    if (isNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvWait))) {
        setNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvChanceWait));
    }
}

void TombSpiderVitalSpot::endChance() {
    if (isNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvChanceWait))) {
        setNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvWait));
    }
}

void TombSpiderVitalSpot::startDamage() {
    setNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvDamageStart));
}

void TombSpiderVitalSpot::recover() {
    setNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvRecover));
}

bool TombSpiderVitalSpot::isDamage() const {
    if (isNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvDamageStart)) ||
        isNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvDamageWait))) {
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
        MR::startSound(this, "SE_BM_TSPIDER_DAMAGE_VITAL");
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
        setNerve(GET_NERVE(TombSpiderVitalSpot, TombSpiderVitalSpotNrvWait));
    }
}

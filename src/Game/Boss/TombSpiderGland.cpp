#include "Game/Boss/TombSpiderGland.hpp"
#include "Game/Scene/SceneFunction.hpp"

namespace NrvTombSpiderGland {
    NEW_NERVE(TombSpiderGlandNrvWait, TombSpiderGland, Wait);
    NEW_NERVE(TombSpiderGlandNrvActiveWait, TombSpiderGland, ActiveWait);
    NEW_NERVE(TombSpiderGlandNrvDamageStart, TombSpiderGland, DamageStart);
    NEW_NERVE(TombSpiderGlandNrvDamageWait, TombSpiderGland, DamageWait);
}  // namespace NrvTombSpiderGland

TombSpiderGland::TombSpiderGland(LiveActor* pActor, const char* pName, const TVec3f& rPos, const TVec3f& a1, const char* pJointName)
    : PartsModel(pActor, pName, "TombSpiderGland", nullptr, MR::DrawBufferType_Enemy, false) {
    MR::initLightCtrl(this);
    initFixedPosition(rPos, a1, pJointName);
}

void TombSpiderGland::init(const JMapInfoIter& rIter) {
    PartsModel::init(rIter);
    MR::startBrk(this, "Battle1st");
    initNerve(&NrvTombSpiderGland::TombSpiderGlandNrvWait::sInstance);
}

void TombSpiderGland::startActive() {
    MR::startBrk(this, "Battle2nd");
    setNerve(&NrvTombSpiderGland::TombSpiderGlandNrvActiveWait::sInstance);
}

void TombSpiderGland::startDamage() {
    setNerve(&NrvTombSpiderGland::TombSpiderGlandNrvDamageStart::sInstance);
}

bool TombSpiderGland::isActive() const {
    return isNerve(&NrvTombSpiderGland::TombSpiderGlandNrvActiveWait::sInstance);
}

bool TombSpiderGland::isDamage() const {
    if (isNerve(&NrvTombSpiderGland::TombSpiderGlandNrvDamageStart::sInstance) ||
        isNerve(&NrvTombSpiderGland::TombSpiderGlandNrvDamageWait::sInstance)) {
        return true;
    }
    return false;
}

void TombSpiderGland::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void TombSpiderGland::exeActiveWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }
}

void TombSpiderGland::exeDamageStart() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DamageStart", nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvTombSpiderGland::TombSpiderGlandNrvDamageWait::sInstance);
    }
}

void TombSpiderGland::exeDamageWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "DamageWait", nullptr);
    }
}

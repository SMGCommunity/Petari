#include "Game/Enemy/WalkerStateBlowDamage.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

void WalkerStateBlowDamage_FORCE_MATCH_SDATA2() {
    (void)1.0f;
}

namespace {
    static const f32 sAirFric = 0.99f;
    static const f32 sAirGravityAccel = 1.0f;
    static const f32 sDamageTurnLimit = 15.0f;
    static const s32 sDamageLandTime = 30;
};  // namespace

namespace NrvWalkerStateBlowDamage {
    NEW_NERVE(WalkerStateBlowDamageNrvBlow, WalkerStateBlowDamage, Blow);
    NEW_NERVE(WalkerStateBlowDamageNrvBlowLand, WalkerStateBlowDamage, BlowLand);
};  // namespace NrvWalkerStateBlowDamage

WalkerStateBlowDamage::WalkerStateBlowDamage(LiveActor* pHost, TVec3f* pDirection, WalkerStateBlowDamageParam* pBlowDamageParam)
    : ActorStateBase< LiveActor >("吹き飛びダメージ状態", pHost), mDirection(pDirection), mBlowDamageParam(pBlowDamageParam) {
    initNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}

void WalkerStateBlowDamage::appear() {
    mIsDead = false;
    setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}

void WalkerStateBlowDamage::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(getHost(), "Damage");
    }
    MR::attenuateVelocity(getHost(), ::sAirFric);
    MR::addVelocityToGravity(getHost(), ::sAirGravityAccel);
    MR::turnDirectionDegree(getHost(), mDirection, -getHost()->mVelocity, ::sDamageTurnLimit);

    if (MR::isGreaterStep(this, 5)) {
        if (MR::isBindedGround(getHost())) {
            MR::startAction(getHost(), "DamageLand");
            MR::zeroVelocity(getHost());
            setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlowLand::sInstance);
        }
    }
}

void WalkerStateBlowDamage::exeBlowLand() {
    if (MR::isGreaterStep(this, ::sDamageLandTime)) {
        kill();
    }
}

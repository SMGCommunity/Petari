#include "Game/Enemy/WalkerStateBlowDamage.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"

namespace NrvWalkerStateBlowDamage {
    NEW_NERVE(WalkerStateBlowDamageNrvBlow, WalkerStateBlowDamage, Blow);
    NEW_NERVE(WalkerStateBlowDamageNrvBlowLand, WalkerStateBlowDamage, BlowLand);
}  // namespace NrvWalkerStateBlowDamage

WalkerStateBlowDamage::WalkerStateBlowDamage(LiveActor* pActor, TVec3f* pVec, WalkerStateBlowDamageParam* pBlowDamageParam)
    : ActorStateBase< LiveActor >("吹き飛びダメージ状態", pActor) {
    initNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}
WalkerStateBlowDamage::~WalkerStateBlowDamage() {}

void WalkerStateBlowDamage::appear() {
    mIsDead = 0;
    setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}

void WalkerStateBlowDamage::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this->mHost, "Damage");
    }
    MR::attenuateVelocity(this->mHost, 0.99f);
    MR::addVelocityToGravity(this->mHost, 1.0f);

    TVec3f mVelocityNegate;
    JMathInlineVEC::PSVECNegate(&mHost->mVelocity, &mVelocityNegate);
    MR::turnDirectionDegree(this->mHost, this->_10, mVelocityNegate, 15.0f);

    if (MR::isGreaterStep(this, 5)) {
        if (MR::isBindedGround(this->mHost)) {
            MR::startAction(this->mHost, "DamageLand");
            MR::zeroVelocity(this->mHost);
            setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlowLand::sInstance);
        }
    }
}

inline void WalkerStateBlowDamage::exeBlowLand() {
    if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}

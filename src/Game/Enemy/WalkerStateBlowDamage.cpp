#include "Game/Enemy/WalkerStateBlowDamage.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "revolution/types.h"

namespace NrvWalkerStateBlowDamage {
    NEW_NERVE(WalkerStateBlowDamageNrvBlow, WalkerStateBlowDamage, Blow);
    NEW_NERVE(WalkerStateBlowDamageNrvBlowLand, WalkerStateBlowDamage, BlowLand);
} // namespace NrvWalkerStateBlowDamage

WalkerStateBlowDamage::WalkerStateBlowDamage(LiveActor* pActor, TVec3f* pVec, WalkerStateBlowDamageParam* pBlowDamageParam)
    : ActorStateBase<LiveActor>("吹き飛びダメージ状態") {
    initNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}
WalkerStateBlowDamage::~WalkerStateBlowDamage() {}

void WalkerStateBlowDamage::appear() {
    mIsDead = 0;
    setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlow::sInstance);
}

void WalkerStateBlowDamage::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this->mParentActor, "Damage");
    }
    MR::attenuateVelocity(this->mParentActor, 0.99f);
    MR::addVelocityToGravity(this->mParentActor, 1.0f);

    TVec3f mVelocityNegate;
    JMathInlineVEC::PSVECNegate(&mParentActor->mVelocity, &mVelocityNegate);
    MR::turnDirectionDegree(this->mParentActor, this->_10, mVelocityNegate, 15.0f);

    if (MR::isGreaterStep(this, 5)) {
        if (MR::isBindedGround(this->mParentActor)) {
            MR::startAction(this->mParentActor, "DamageLand");
            MR::zeroVelocity(this->mParentActor);
            setNerve(&NrvWalkerStateBlowDamage::WalkerStateBlowDamageNrvBlowLand::sInstance);
        }
    }
}

inline void WalkerStateBlowDamage::exeBlowLand() {
    if (MR::isGreaterStep(this, 30)) {
        kill();
    }
}

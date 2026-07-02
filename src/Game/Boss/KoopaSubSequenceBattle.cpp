#include "Game/Boss/KoopaSubSequenceBattle.hpp"
#include "Game/Boss/Koopa.hpp"
#include "Game/Boss/KoopaBattleBase.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaSequencer.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/NerveUtil.hpp"

namespace NrvKoopaSubSequenceBattle {
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv1, KoopaSubSequenceBattle, BattleLv1);
    NEW_NERVE(KoopaSubSequenceBattleNrvDemoStartLv2, KoopaSubSequenceBattle, DemoStartLv2);
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv2, KoopaSubSequenceBattle, BattleLv2);
    NEW_NERVE(KoopaSubSequenceBattleNrvDemoStartLv3, KoopaSubSequenceBattle, DemoStartLv3);
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv3, KoopaSubSequenceBattle, BattleLv3);
};  // namespace NrvKoopaSubSequenceBattle

KoopaSubSequenceBattle::KoopaSubSequenceBattle(Koopa* pKoopa, KoopaSequencer* pSequencer, KoopaBattleBase* pBattleBase)
    : ActorStateBase< Koopa >("Sequence[戦闘]", pKoopa), mSequencer(pSequencer), mBattleBase(pBattleBase) {
    initNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv1::sInstance);
}

void KoopaSubSequenceBattle::kill() {
    mIsDead = true;

    MR::offSwitchB(mHost);
}

bool KoopaSubSequenceBattle::isDemo() const {
    if (isNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv2::sInstance) ||
        isNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv3::sInstance)) {
        return true;
    }

    return false;
}

void KoopaSubSequenceBattle::exeBattleLv1() {
    MR::updateActorStateAndNextNerve(this, mBattleBase, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv2::sInstance);
}

void KoopaSubSequenceBattle::exeDemoStartLv2() {
    if (MR::isFirstStep(this)) {
        mSequencer->startBattleLevel2();
    }

    MR::updateActorStateAndNextNerve(this, mSequencer->mPowerUp, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv2::sInstance);
}

void KoopaSubSequenceBattle::exeBattleLv2() {
    MR::updateActorStateAndNextNerve(this, mBattleBase, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv3::sInstance);
}

void KoopaSubSequenceBattle::exeDemoStartLv3() {
    if (MR::isFirstStep(this)) {
        mSequencer->startBattleLevel3();
    }

    MR::updateActorStateAndNextNerve(this, mSequencer->mPowerUp, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv3::sInstance);
}

void KoopaSubSequenceBattle::exeBattleLv3() {
    if (MR::updateActorState(this, mBattleBase)) {
        kill();
    }
}

KoopaSubSequenceBattle::~KoopaSubSequenceBattle() {
}

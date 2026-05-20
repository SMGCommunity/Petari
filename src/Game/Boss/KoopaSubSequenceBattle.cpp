#include "Game/Boss/KoopaSubSequenceBattle.hpp"
#include "Game/Boss/KoopaDemoPowerUp.hpp"
#include "Game/Boss/KoopaSequencer.hpp"

namespace NrvKoopaSubSequenceBattle {
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv1, KoopaSubSequenceBattle, BattleLv1);
    NEW_NERVE(KoopaSubSequenceBattleNrvDemoStartLv2, KoopaSubSequenceBattle, DemoStartLv2);
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv2, KoopaSubSequenceBattle, BattleLv2);
    NEW_NERVE(KoopaSubSequenceBattleNrvDemoStartLv3, KoopaSubSequenceBattle, DemoStartLv3);
    NEW_NERVE(KoopaSubSequenceBattleNrvBattleLv3, KoopaSubSequenceBattle, BattleLv3);
}  // namespace NrvKoopaSubSequenceBattle

KoopaSubSequenceBattle::KoopaSubSequenceBattle(Koopa* pKoopa, KoopaSequencer* pSequencer, KoopaBattleBase* pBattleBase)
    : ActorStateBase< Koopa >("Sequence[戦闘]", pKoopa), mSequencer(pSequencer), mBattleBase(pBattleBase) {
    initNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv1::sInstance);
}

KoopaSubSequenceBattle::~KoopaSubSequenceBattle() {
}

void KoopaSubSequenceBattle::kill() {
    mIsDead = true;
    MR::offSwitchB(mHost);
}

bool KoopaSubSequenceBattle::isDemo() {
    if (isNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv2::sInstance) ||
        isNerve(&NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvDemoStartLv3::sInstance)) {
        return true;
    }

    return false;
}

void KoopaSubSequenceBattle::exeBattleLv1() {
}

void KoopaSubSequenceBattle::exeDemoStartLv2() {
    if (MR::isFirstStep(this)) {
        mSequencer->startBattleLevel2();
    }

    MR::updateActorStateAndNextNerve(this, mSequencer->mPowerUp, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv2::sInstance);
}

void KoopaSubSequenceBattle::exeBattleLv2() {
}

void KoopaSubSequenceBattle::exeDemoStartLv3() {
    if (MR::isFirstStep(this)) {
        mSequencer->startBattleLevel3();
    }

    MR::updateActorStateAndNextNerve(this, mSequencer->mPowerUp, &NrvKoopaSubSequenceBattle::KoopaSubSequenceBattleNrvBattleLv3::sInstance);
}

void KoopaSubSequenceBattle::exeBattleLv3() {
}

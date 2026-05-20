#include "Game/Boss/KoopaSequencerVs1.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvKoopaSequencerVs1 {
    NEW_NERVE(KoopaSequencerVs1NrvBattleStairs, KoopaSequencerVs1, BattleStairs);
    NEW_NERVE(KoopaSequencerVs1NrvDemoBattleStart, KoopaSequencerVs1, DemoBattleStart);
    NEW_NERVE(KoopaSequencerVs1NrvDemoJumpToPlanet, KoopaSequencerVs1, DemoJumpToPlanet);
    NEW_NERVE(KoopaSequencerVs1NrvBattle, KoopaSequencerVs1, Battle);
    NEW_NERVE(KoopaSequencerVs1NrvWaitDemoBattleEnd, KoopaSequencerVs1, WaitDemoBattleEnd);
    NEW_NERVE(KoopaSequencerVs1NrvDemoBattleEnd, KoopaSequencerVs1, DemoBattleEnd);
    NEW_NERVE(KoopaSequencerVs1NrvWaitDemo, KoopaSequencerVs1, WaitDemo);
}  // namespace NrvKoopaSequencerVs1

KoopaSequencerVs1::KoopaSequencerVs1() : KoopaSequencer("クッパ戦（Ｖｓ１進行役）", 1), _24(), _28() {
}

KoopaSequencerVs1::~KoopaSequencerVs1() {
}

void KoopaSequencerVs1::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
}

void KoopaSequencerVs1::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}

void KoopaSequencerVs1::receiveMsgPlayerAttack(u32, HitSensor* pSender, HitSensor* pReceiver) {
}

void KoopaSequencerVs1::receiveMsgEnemyAttack(u32, HitSensor* pSender, HitSensor* pReceiver) {
}

void KoopaSequencerVs1::receiveOtherMsg(u32, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvKoopaSequencerVs1::KoopaSequencerVs1NrvBattle::sInstance)) {
    }
}

void KoopaSequencerVs1::exeBattleStairs() {
}

void KoopaSequencerVs1::exeDemoBattleStart() {
}

void KoopaSequencerVs1::exeDemoJumpToPlanet() {
}

void KoopaSequencerVs1::exeBattle() {
}

void KoopaSequencerVs1::exeWaitDemoBattleEnd() {
}

void KoopaSequencerVs1::exeDemoBattleEnd() {
}

void KoopaSequencerVs1::exeWaitDemo() {
}

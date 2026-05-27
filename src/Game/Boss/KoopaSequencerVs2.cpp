#include "Game/Boss/KoopaSequencerVs2.hpp"

KoopaSequencerVs2::KoopaSequencerVs2() : KoopaSequencer("クッパ戦（Ｖｓ２進行役）", 2) {
}

KoopaSequencerVs2::~KoopaSequencerVs2() {
}

void KoopaSequencerVs2::init(Koopa* pKoopa, const JMapInfoIter& rIter) {
}

bool KoopaSequencerVs2::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}

bool KoopaSequencerVs2::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
}

bool KoopaSequencerVs2::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
}

bool KoopaSequencerVs2::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
}

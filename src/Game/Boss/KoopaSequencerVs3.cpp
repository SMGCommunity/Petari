#include "Game/Boss/KoopaSequencerVs3.hpp"

KoopaSequencerVs3::KoopaSequencerVs3() : KoopaSequencer("クッパ戦（Ｖｓ３進行役）", 3) {
}

KoopaSequencerVs3::~KoopaSequencerVs3() {
}

void KoopaSequencerVs3::init(Koopa*, const JMapInfoIter&) {
}

void calcAndSetBaseMtx() {
}

bool KoopaSequencerVs3::attackSensor(HitSensor*, HitSensor*) {
}

bool KoopaSequencerVs3::receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*) {
}

bool KoopaSequencerVs3::receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*) {
}

bool KoopaSequencerVs3::receiveOtherMsg(u32, HitSensor*, HitSensor*) {
}

#include "Game/Boss/BossKameckSequencer.hpp"
#include "Game/Boss/BossKameckBattleDemo.hpp"

BossKameckSequencer::BossKameckSequencer(const char *pName) : NerveExecutor(pName) {
    mBossKameck = nullptr;
    _C = 0;
    mBattleDemo = nullptr;
}

void BossKameckSequencer::init(BossKameck *pBoss, const JMapInfoIter &rIter) {
    mBossKameck = pBoss;
    mBattleDemo = new BossKameckBattleDemo(pBoss, rIter);
    mBattleDemo->init();
}

void BossKameckSequencer::update() {
    updateNerve();
}

/* 
 * There is no way to know what they wanted at offset 0xC in this class.
 * It is always nullptr so the sensors never react.
 * This is most likely a remenant of another class that they accidentally included (or left) here,
 * so this code is my best guess since the vtables align up. It is some sort of BossKameckAction. 
*/
void BossKameckSequencer::attackSensor(HitSensor *a1,  HitSensor *a2) {
    if (_C != nullptr) {
        _C->attackSensor(a1, a2);
    }
}

bool BossKameckSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (_C != nullptr) {
        return _C->receiveMsgPlayerAttack(msg, a1, a2);
    }

    return false;
}

bool BossKameckSequencer::receiveMsgEnemyAttack(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (_C != nullptr) {
        return _C->receiveMsgEnemyAttack(msg, a1, a2);
    }

    return false;
}

bool BossKameckSequencer::receiveMsgPush(HitSensor *a1, HitSensor *a2) {
    if (_C != nullptr) {
        return _C->receiveMsgPush(a1, a2);
    }

    return false;
}

bool BossKameckSequencer::receiveOtherMsg(u32 msg, HitSensor *a1, HitSensor *a2) {
    if (_C != nullptr) {
        return _C->receiveOtherMsg(msg, a1, a2);
    }

    return false;
}

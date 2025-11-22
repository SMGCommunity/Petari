#include "Game/Boss/BossKameckAction.hpp"

BossKameckAction::BossKameckAction(const char* pName, BossKameck* pBoss)
    : ActorStateBase< BossKameck >(pName) {
    mBossKameck = pBoss;
}

BossKameckAction::~BossKameckAction() {
}

bool BossKameckAction::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BossKameckAction::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BossKameckAction::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool BossKameckAction::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

void BossKameckAction::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
}
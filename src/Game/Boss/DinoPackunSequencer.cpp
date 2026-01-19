#include "Game/Boss/DinoPackunSequencer.hpp"
#include "Game/Boss/DinoPackunAction.hpp"
#include "Game/LiveActor/HitSensor.hpp"

DinoPackunSequencer::DinoPackunSequencer(const char* pName, DinoPackun* pParent) : NerveExecutor(pName) {
    mParent = pParent;
    mCurrentAction = nullptr;
}

void DinoPackunSequencer::update() {
    updateNerve();
}

void DinoPackunSequencer::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        mCurrentAction->attackSensor(pSender, pReceiver);
    }
}

bool DinoPackunSequencer::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool DinoPackunSequencer::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }

    return false;
}

bool DinoPackunSequencer::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgPush(pSender, pReceiver);
    }

    return false;
}

bool DinoPackunSequencer::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveOtherMsg(msg, pSender, pReceiver);
    }

    return false;
}

void DinoPackunSequencer::attackSensorTail(HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        mCurrentAction->attackSensorTail(pSender, pReceiver);
    }
}

bool DinoPackunSequencer::receiveMsgPlayerAttackTail(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveMsgPlayerAttackTail(msg, pSender, pReceiver);
    }

    return false;
}

bool DinoPackunSequencer::receiveOtherMsgTail(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (mCurrentAction != nullptr) {
        return mCurrentAction->receiveOtherMsgTail(msg, pSender, pReceiver);
    }

    return false;
}

DinoPackunSequencer::~DinoPackunSequencer() {}

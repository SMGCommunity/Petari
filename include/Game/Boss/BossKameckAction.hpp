#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class BossKameck;
class HitSensor;

class BossKameckAction : public ActorStateBase< BossKameck > {
public:
    BossKameckAction(const char*, BossKameck*);

    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    }

    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    /* 0x28 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    /* 0x2C */ virtual bool receiveMsgPush(HitSensor*, HitSensor*) {
        return false;
    }

    /* 0x30 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }
};

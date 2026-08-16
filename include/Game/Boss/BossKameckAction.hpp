#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class BossKameck;
class HitSensor;

class BossKameckAction : public ActorStateBase< BossKameck > {
public:
    BossKameckAction(const char*, BossKameck*);

    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    }

    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }

    virtual bool receiveMsgPush(HitSensor*, HitSensor*) {
        return false;
    }

    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
        return false;
    }
};

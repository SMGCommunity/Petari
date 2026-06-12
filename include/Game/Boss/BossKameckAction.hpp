#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class BossKameck;
class HitSensor;

class BossKameckAction : public ActorStateBase< BossKameck > {
public:
    BossKameckAction(const char*, BossKameck*);

    virtual ~BossKameckAction();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
};

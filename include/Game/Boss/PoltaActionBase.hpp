#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class Polta;
class HitSensor;

class PoltaActionBase : public ActorStateBase< Polta > {
public:
    PoltaActionBase(const char*, Polta*);

    /* 0x08 */ virtual ~PoltaActionBase();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x28 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool updateWait();
    bool updateDamageBody(bool);
    bool updateBreakBody();
    bool updateDamageCore();
    void updateScreamSensor();

    s32 _10;
};

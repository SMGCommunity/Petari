#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;

class DinoPackunBattleEgg : public DinoPackunAction {
public:
    DinoPackunBattleEgg(DinoPackun*);

    virtual ~DinoPackunBattleEgg();
    virtual void init();
    virtual void appear();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeTurn();
    void exeWalk();
    void exeDamage();

    DinoPackunStateDamage* mStateDamage;  // 0x14
    f32 _18;
};

#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;

class DinoPackunBattleVs1Lv1 : public DinoPackunAction {
public:
    DinoPackunBattleVs1Lv1(DinoPackun*);

    virtual ~DinoPackunBattleVs1Lv1();
    virtual void appear();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    bool tryFind();

    void exeStart();
    void exeTurn();
    void exeWalk();
    void exeFind();
    void exeChase();
    void exeCoolDown();
    void exeAttackHit();
    void exeDamage();

    /* 0x14 */ DinoPackunStateDamage* mStateDamage;
};

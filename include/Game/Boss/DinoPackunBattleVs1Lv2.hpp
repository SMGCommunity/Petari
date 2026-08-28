#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;

class DinoPackunBattleVs1Lv2 : public DinoPackunAction {
public:
    DinoPackunBattleVs1Lv2(DinoPackun*);

    virtual ~DinoPackunBattleVs1Lv2();
    /* 0x10 */ virtual void appear();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x30 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setMiddleBattle();
    void setLastBattle();
    void setStartFromDemo();
    void setStartContinueBattle();
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
    /* 0x18 */ bool _18;
};

#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;

class DinoPackunBattleEgg : public DinoPackunAction {
public:
    DinoPackunBattleEgg(DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunBattleEgg();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x30 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeTurn();
    void exeWalk();
    void exeDamage();

    DinoPackunStateDamage* mStateDamage;  // 0x14
    f32 _18;
};

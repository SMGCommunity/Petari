#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;
class DinoPackunStateFire;
class DinoPackunTrackFireHolder;

class DinoPackunBattleEggVs2 : public DinoPackunAction {
public:
    DinoPackunBattleEggVs2(DinoPackun*);

    virtual ~DinoPackunBattleEggVs2();
    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x1C */ virtual void control();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x30 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setTrackFire(DinoPackunTrackFireHolder*);
    void exeTurn();
    void exeWalk();
    void exeDamage();

    DinoPackunStateDamage* mStateDamage;          // 0x14
    DinoPackunTrackFireHolder* mTrackFireHolder;  // 0x18
    DinoPackunStateFire* mStateFire;              // 0x1C
    f32 _20;
};

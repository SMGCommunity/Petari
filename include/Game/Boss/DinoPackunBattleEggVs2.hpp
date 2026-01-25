#pragma once

#include "Game/Boss/DinoPackunAction.hpp"

class DinoPackunStateDamage;
class DinoPackunStateFire;
class DinoPackunTrackFireHolder;

class DinoPackunBattleEggVs2 : public DinoPackunAction {
public:
    DinoPackunBattleEggVs2(DinoPackun*);

    virtual ~DinoPackunBattleEggVs2();
    virtual void init();
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void setTrackFire(DinoPackunTrackFireHolder*);
    void exeTurn();
    void exeWalk();
    void exeDamage();

    DinoPackunStateDamage* mStateDamage;          // 0x14
    DinoPackunTrackFireHolder* mTrackFireHolder;  // 0x18
    DinoPackunStateFire* mStateFire;              // 0x1C
};

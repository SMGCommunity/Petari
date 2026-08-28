#pragma once

#include "Game/Boss/TombSpiderActionBase.hpp"

class TombSpider;
class HitSensor;

class TombSpiderAction2nd : public TombSpiderActionBase {
public:
    TombSpiderAction2nd(TombSpider*);

    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeAttackStart();
    void exeAttackLoop();
    void exeAttackEnd();
    void exeDamageEye();
    void exeDamageHip();
    void exeDamageGland();
    void exeSwoon();
    void exeChanceStart();
    void exeChanceWait();
    void exeChanceDamage();
    void exeChanceDamageLast();
    void exeChanceEnd();
    void exeWaitDemo();
};

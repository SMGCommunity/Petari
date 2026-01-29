#pragma once

#include "Game/Boss/TombSpiderActionBase.hpp"

class TombSpider;
class HitSensor;

class TombSpiderAction1st : public TombSpiderActionBase {
public:
    TombSpiderAction1st(TombSpider*);

    virtual void init();
    virtual void appear();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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

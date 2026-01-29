#pragma once

#include "Game/Boss/TombSpiderActionBase.hpp"

class TombSpider;
class HitSensor;

class TombSpiderActionCocoon : public TombSpiderActionBase {
public:
    TombSpiderActionCocoon(TombSpider*);

    virtual void init();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeBreak();
};

#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SwitchBox : public LiveActor {
public:
    SwitchBox(const char* pName);

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void appear();
    virtual void kill();
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeHit();
    void doHit(HitSensor* pSender, HitSensor* pReceiver);

    u16 _8c;
    u16 _8e;
    u16 _90;
    u8 _92;
};

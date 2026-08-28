#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SwitchBox : public LiveActor {
public:
    SwitchBox(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeHit();
    void doHit(HitSensor* pSender, HitSensor* pReceiver);

    u16 _8c;
    u16 _8e;
    u16 _90;
    u8 _92;
    u8 _93[5];
};

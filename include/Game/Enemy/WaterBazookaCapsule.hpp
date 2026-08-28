#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class WaterBazookaCapsule : public PartsModel {
public:
    WaterBazookaCapsule(LiveActor*, const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void crackCapsule();
    void breakCapsule();
    bool isPlayerOnCapsule() const;

    void exeNormal();
    void exeCrack();
    void exeBreak();
};

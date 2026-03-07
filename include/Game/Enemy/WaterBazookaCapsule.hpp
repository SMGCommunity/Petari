#pragma once

#include "Game/LiveActor/PartsModel.hpp"

class WaterBazookaCapsule : public PartsModel {
public:
    WaterBazookaCapsule(LiveActor*, const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void crackCapsule();
    void breakCapsule();
    bool isPlayerOnCapsule() const;

    void exeNormal();
    void exeCrack();
    void exeBreak();
};

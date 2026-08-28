#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class Jiraira : public LiveActor {
public:
    Jiraira(const char*);

    /* 0x08 */ virtual ~Jiraira();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeStepped();
    void exeExplode();
    void exePreRecover();
    void exeRecover();

    CollisionParts* _8C;
    f32 _90;
};

#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;
class HitSensor;

class DinoPackunAction : public ActorStateBase< DinoPackun > {
public:
    DinoPackunAction(const char*, DinoPackun*);

    virtual ~DinoPackunAction();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void attackSensorTail(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsgTail(u32, HitSensor*, HitSensor*);

    bool sendBlowAttackMessage(HitSensor*, HitSensor*, bool);
    bool sendHitAttackMessage(HitSensor*, HitSensor*, bool);
    void updateTurn(s32, f32);
    bool updateWalk(s32, f32, s32);
    bool updateChase(s32, f32, f32, f32, s32, s32);
    void selectTurnDirection();
    bool updateStart();
    bool updateFind(s32, f32);
    bool updateCoolDown(s32);
    bool updateAttackHit();

    DinoPackun* mParent;  // 0x0C
    f32 _10;
};

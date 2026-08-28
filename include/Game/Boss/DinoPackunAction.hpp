#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"

class DinoPackun;
class HitSensor;

class DinoPackunAction : public ActorStateBase< DinoPackun > {
public:
    DinoPackunAction(const char*, DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunAction();
    /* 0x20 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x28 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x30 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x34 */ virtual void attackSensorTail(HitSensor*, HitSensor*);
    /* 0x38 */ virtual bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
    /* 0x3C */ virtual bool receiveOtherMsgTail(u32, HitSensor*, HitSensor*);

    bool sendBlowAttackMessage(HitSensor*, HitSensor*, bool);
    bool sendHitAttackMessage(HitSensor*, HitSensor*, bool);
    bool updateTurn(s32, f32);
    bool updateWalk(s32, f32, s32);
    bool updateChase(s32, f32, f32, f32, s32, s32);
    void selectTurnDirection();
    bool updateStart();
    bool updateFind(s32, f32);
    bool updateCoolDown(s32);
    bool updateAttackHit();

    f32 _10;
};

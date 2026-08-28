#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class BossStinkBugActionGround : public BossStinkBugActionBase {
public:
    BossStinkBugActionGround(BossStinkBug*);

    /* 0x10 */ virtual void appear();
    /* 0x1C */ virtual void control();
    /* 0x20 */ virtual bool isValidFollowId(s32 id) const;
    /* 0x24 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x28 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeStart();
    void exeWalk();
    void exeDamage();
    void exeFind();
    void exeShakeOff();
    void exeShakeOffRun();
    void exeRun();
    void exeAttackSuccess();
    void endAttackSuccess();
    void exeCoolDown();
    void exeTurn();
    bool isEnableHipDrop() const;
    bool isEnableShakeStart() const;
};

#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class BossStinkBugActionGround : public BossStinkBugActionBase {
public:
    BossStinkBugActionGround(BossStinkBug*);
    
    virtual void appear();
    virtual void control();
    virtual bool isValidFollowId(s32 id) const;
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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

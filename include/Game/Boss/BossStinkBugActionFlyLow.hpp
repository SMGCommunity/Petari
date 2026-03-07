#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class BossStinkBugActionFlyLow : public BossStinkBugActionBase {
public:
    BossStinkBugActionFlyLow(BossStinkBug* pStinkBug);

    virtual void appear();
    virtual void control();
    virtual bool isValidFollowId(s32 id) const;
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    
    void exeFly();
    void exeShakeOffSign();
    void exeShakeOff();
    void exeShakeOffWait();
    void exeShakeOffEnd();
    void exeFall();
    void exeDamage();
    void exeDash();
    void exeTurnSign();
    void exeTurn();
    bool isEnableHipDrop() const;
    bool isEnableAttack() const;
    
    f32 _64;
};

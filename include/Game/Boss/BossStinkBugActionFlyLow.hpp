#pragma once

#include "Game/Boss/BossStinkBugActionBase.hpp"

class BossStinkBugActionFlyLow : public BossStinkBugActionBase {
public:
    BossStinkBugActionFlyLow(BossStinkBug* pStinkBug);

    /* 0x10 */ virtual void appear();
    /* 0x1C */ virtual void control();
    /* 0x20 */ virtual bool isValidFollowId(s32 id) const;
    /* 0x28 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

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

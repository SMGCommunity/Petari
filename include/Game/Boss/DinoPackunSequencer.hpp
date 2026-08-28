#pragma once

#include "Game/System/NerveExecutor.hpp"

class DinoPackun;
class HitSensor;
class DinoPackunAction;

class DinoPackunSequencer : public NerveExecutor {
public:
    DinoPackunSequencer(const char*, DinoPackun*);

    /* 0x08 */ virtual ~DinoPackunSequencer();

    /* 0x0C */ virtual void start() {
        return;
    }
    /* 0x10 */ virtual void init() {
        return;
    }
    /* 0x14 */ virtual bool isUseEggShell() const {
        return true;
    }
    virtual s32 getVsCount() const {
        return 0;
    }
    /* 0x18 */ virtual void update();
    /* 0x1C */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x20 */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x24 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x28 */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x2C */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    /* 0x30 */ virtual void attackSensorTail(HitSensor*, HitSensor*);
    /* 0x34 */ virtual bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
    /* 0x38 */ virtual bool receiveOtherMsgTail(u32, HitSensor*, HitSensor*);

    DinoPackun* mParent;               // 0x8
    DinoPackunAction* mCurrentAction;  // 0xC
};

#pragma once

#include "Game/System/NerveExecutor.hpp"

class DinoPackun;
class HitSensor;
class DinoPackunAction;

class DinoPackunSequencer : public NerveExecutor {
public:
    DinoPackunSequencer(const char*, DinoPackun*);

    virtual ~DinoPackunSequencer();
    virtual void start();
    virtual void init();
    virtual bool isUseEggShell() const;
    virtual u32 getVsCount() const;
    virtual void update();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    virtual void attackSensorTail(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttackTail(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsgTail(u32, HitSensor*, HitSensor*);

    DinoPackun* mParent;               // 0x8
    DinoPackunAction* mCurrentAction;  // 0xC
};
